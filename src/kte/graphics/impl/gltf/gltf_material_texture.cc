#include "kte/graphics/impl/gltf/gltf_material_texture.h"
#include "kte/graphics/gl_internals.h"

#include <glm/gtc/type_ptr.hpp>

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
struct GLTFMaterialTexture::Data
{
  GLint texture_unit = -1;
};

////////////////////////////////////////////////////////////////
GLTFMaterialTexture::GLTFMaterialTexture()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
GLTFMaterialTexture::~GLTFMaterialTexture() = default;

////////////////////////////////////////////////////////////////
void GLTFMaterialTexture::setupProgram(uint8_t texture_unit,
                                       const ProgramInfo& info)
{
  if (!texture_)
  {
    return;
  }

  texture_->load();

  data_->texture_unit = texture_unit;
  if (data_->texture_unit >= 0)
  {
    glActiveTexture(GL_TEXTURE0 + data_->texture_unit);
    checkErrors("glActiveTexture");
  }

  const auto& texture_uniforms = info.texture_uniforms.at(uniform_name_);

  glBindTexture(GL_TEXTURE_2D, texture_->internal_id());
  checkErrors("glBindTexture");

  const auto texture_uniform = texture_uniforms.texture;
  if (texture_uniform >= 0 && data_->texture_unit >= 0)
  {
    glUniform1i(texture_uniform, data_->texture_unit);
    checkErrors("glUniform1i texture");
  }

  const auto factor_uniform = texture_uniforms.factor;
  if (factor_uniform >= 0)
  {
    glUniform4fv(factor_uniform, 1, glm::value_ptr(factor_));
    checkErrors("glUniform1i scale");
  }

  const auto tex_coord_uniform = texture_uniforms.tex_coord;
  if (tex_coord_uniform >= 0)
  {
    glUniform1i(tex_coord_uniform, tex_coord_);
    checkErrors("glUniform1i texCoord");
  }
}

} // end namespace graphics
} // end namespace kte

