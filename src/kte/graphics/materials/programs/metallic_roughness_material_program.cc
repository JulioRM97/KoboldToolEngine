#include "metallic_roughness_material_program.h"
#include "kte/graphics/gl_internals.h"

#include <glm/gtc/type_ptr.hpp>

#include "kte/game/components/light_component.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
MetallicRoughnessMaterialProgram::MetallicRoughnessMaterialProgram()
{
  data_->base_shader_name = "metallic_roughness_material";

  // These names are defined by GLTF

  data_->required_textures = {"baseColor", "metallicRoughness", "emissive",
                              "occlusion", "normals"};

  data_->required_factors = {"metallicFactor", "roughnessFactor"};

  data_->required_attributes = {{"POSITION", "a_position"},
                                {"NORMAL", "a_normal"},
                                {"TANGENT", "a_tangent"},
                                {"TEXCOORD_0", "a_texCoord_0"},
                                {"TEXCOORD_1", "a_texCoord_1"},
                                {"COLOR_0", "a_v3color_0"},
                                {"JOINTS_0", "a_joints_0"},
                                {"WEIGHTS_0", "a_weights_0"}};

  data_->required_uniforms = {{"PROJECTION", "u_m_projection"},
                              {"VIEW", "u_m_view"},
                              {"MODEL", "u_m_model"}};

  data_->programs[RenderMode::k_Forward];
  data_->programs[RenderMode::k_Deferred];
  data_->programs[RenderMode::k_ShadowMap];
  data_->programs[RenderMode::k_ZPrePass];

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
MetallicRoughnessMaterialProgram::~MetallicRoughnessMaterialProgram() = default;

////////////////////////////////////////////////////////////////
void MetallicRoughnessMaterialProgram::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void MetallicRoughnessMaterialProgram::draw(const Drawable& drawable,
                                            const LightComponent* light,
                                            const glm::mat4& model,
                                            const glm::mat4& view,
                                            const glm::mat4& projection,
                                            RenderMode mode) const
{
  const auto& info = data_->programs[mode];

  if (light)
  {
    light->use(info);
  }

  glUseProgram(info.program_id);

  GLint model_id = info.uniforms.at("MODEL");
  if (model_id >= 0)
  {
    glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  }
  checkErrors("glUniformMatrix4fv MODEL");

  GLint proj_id = info.uniforms.at("PROJECTION");
  if (proj_id >= 0)
  {
    glUniformMatrix4fv(proj_id, 1, GL_FALSE, glm::value_ptr(projection));
  }
  checkErrors("glUniformMatrix4fv PROJECTION");

  GLint view_id = info.uniforms.at("VIEW");
  if (view_id >= 0)
  {
    glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  }
  checkErrors("glUniformMatrix4fv VIEW");

  drawable.draw();

  glUseProgram(0);
}

} // end namespace graphics
} // end namespace kte

