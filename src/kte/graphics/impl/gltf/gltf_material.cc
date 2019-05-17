#include "gltf_material.h"
#include "kte/graphics/gl_internals.h"
#include "kte/graphics/impl/gltf/gltf_internals.h"
#include "kte/common.h"

#include <algorithm>

#include <glm/ext.hpp>

#include "kte/game/game_manager.h"
#include "kte/graphics/impl/gltf/gltf_material_texture.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
struct GLTFMaterial::Data
{
  gltf_shared_ptr<tinygltf::Material> material;

  std::map<std::string, std::shared_ptr<GLTFMaterialTexture>> material_textures;
  std::map<std::string, glm::vec4> factors;
};

////////////////////////////////////////////////////////////////
GLTFMaterial::GLTFMaterial()
    : data_(new Data())
{
  program_ = kte::GameManager::Instance().pbr_program_;
}

////////////////////////////////////////////////////////////////
kte::graphics::GLTFMaterial::GLTFMaterial(gltf_shared_ptr<tinygltf::Material> material)
    : data_(new Data())
{
  program_ = kte::GameManager::Instance().pbr_program_;

  data_->material = material;

  for (const auto& name : program_->data_->required_textures)
  {
    data_->material_textures[name] = getTexture(name);
  }

  for (const auto& name : program_->data_->required_factors)
  {
    data_->factors[name] = getFactor(name);
  }
}

////////////////////////////////////////////////////////////////
GLTFMaterial::~GLTFMaterial() = default;

////////////////////////////////////////////////////////////////
void GLTFMaterial::setupProgram(RenderMode mode)
{ $
  program_->load();

  const auto& info = program_->data_->programs[mode];

  glUseProgram(info.program_id);

  for (const auto& factor : data_->factors)
  {
    const auto factor_uniform = info.uniforms.at(factor.first);

    if (factor_uniform != -1)
    {
      glUniform4fv(factor_uniform, 1, glm::value_ptr(factor.second));
      checkErrors("glUniform4fv");
    }
  }

  uint8_t texture_unit = 0;
  for (auto& pair : data_->material_textures)
  {
    pair.second->setupProgram(texture_unit++, info);
  }

  glUseProgram(0);
}

////////////////////////////////////////////////////////////////
std::shared_ptr<GLTFMaterialTexture> GLTFMaterial::getTexture(std::string name)
{
  const tinygltf::ParameterMap* parameter_map = nullptr;
  auto material = data_->material;

  auto texture_name = name + "Texture";
  auto factor_name = name + "Factor";

  if (material->values.count(texture_name) > 0 ||
      material->values.count(factor_name) > 0)
  {
    parameter_map = &material->values;
  }
  else if (material->additionalValues.count(texture_name) > 0 ||
      material->additionalValues.count(factor_name) > 0)
  {
    parameter_map = &material->additionalValues;
  }

  auto mt = std::make_shared<GLTFMaterialTexture>();

  mt->uniform_name_ = name;

  // Read factor, if any
  if (parameter_map && parameter_map->count(factor_name) > 0)
  {
    mt->factor_ = gltfParamToVec4(parameter_map->at(factor_name));
  }

  // Read other material texture properties
  if (parameter_map && parameter_map->count(texture_name) > 0)
  {
    auto gltf_texture_values = &parameter_map->at(texture_name);
    auto values = gltf_texture_values->json_double_value;

    if (values.count("index") == 0)
    {
      fprintf(stderr, "Invalid texture index: %s\n", name.c_str());
      return std::make_shared<GLTFMaterialTexture>();
    }

    mt->texture_ = data_->material.model_data()->textures_[values.at("index")];

    if (values.count("scale") > 0)
    {
      mt->factor_ *= values.at("scale");
    }

    if (values.count("strength") > 0)
    {
      mt->factor_ *= values.at("strength");
    }

    if (values.count("texCoord"))
    {
      mt->tex_coord_ = static_cast<int>(values.at("texCoord"));
    }
  }
  else
  {
    // FIXME create a more simple empty texture for this.
    mt->texture_ = std::make_shared<GLTFTexture>();
  }

  return mt;
}

/////////////////////////////////////////////////////////////////
glm::vec4 GLTFMaterial::getFactor(std::string factor_name)
{
  tinygltf::Parameter factor;
  auto material = data_->material;

  if (material->values.count(factor_name) > 0)
  {
    factor = material->values.at(factor_name);
  }
  else if (material->additionalValues.count(factor_name) > 0)
  {
    factor = material->additionalValues.at(factor_name);
  }
  else if (material->extPBRValues.count(factor_name) > 0)
  {
    factor = material->extPBRValues.at(factor_name);
  }
  else if (material->extCommonValues.count(factor_name) > 0)
  {
    factor = material->extCommonValues.at(factor_name);
  }

  return gltfParamToVec4(factor);
}

} //end namespace graphics
} //end namespace kte
