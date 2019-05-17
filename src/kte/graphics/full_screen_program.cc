#include "kte/graphics/full_screen_program.h"

#include "kte/graphics/gl_internals.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
FullScreenProgram::FullScreenProgram(std::string base_shader_name)
{
  data_->base_shader_name = base_shader_name;
  data_->programs[RenderMode::k_FullScreen];
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
FullScreenProgram::~FullScreenProgram() = default;

////////////////////////////////////////////////////////////////
void FullScreenProgram::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void FullScreenProgram::draw(const Drawable& drawable,
                             const LightComponent* light,
                             const glm::mat4& model,
                             const glm::mat4& view,
                             const glm::mat4& projection,
                             RenderMode mode) const
{
  fprintf(stderr, "A FullScreenProgram should not be used for drawing drawables");
}

////////////////////////////////////////////////////////////////
void FullScreenProgram::setupInputFramebuffer(const Framebuffer& framebuffer,
                                              RenderMode mode)
{
  // TODO these could be something like `required_input_framebuffer_textures`
  // using texture types. We could also setup the input framebuffer depending
  // on those or something similar instead of assuming the linear order (idx)
  // from required_textures.

  auto& info = data_->programs[mode];

  glUseProgram(info.program_id);
  checkErrors("glUseProgram");

  uint8_t idx = 0;
  for (; idx < framebuffer.num_color_textures(); idx++)
  {
    bindTexture(*framebuffer.color_texture(idx), info, idx);
  }

  if (framebuffer.depth_texture())
  {
    bindTexture(*framebuffer.depth_texture(), info, idx++);
  }

  if (framebuffer.stencil_texture())
  {
    bindTexture(*framebuffer.stencil_texture(), info, idx++);
  }

  glUseProgram(0);

  checkErrors("FullScreenProgram::setupInputFramebuffer DONE");
}

////////////////////////////////////////////////////////////////
void FullScreenProgram::bindTexture(const Texture& texture,
                                    const ProgramInfo& info,
                                    uint8_t idx)
{
  if (idx >= data_->required_textures.size())
  {
    return;
  }

  const auto& name = data_->required_textures[idx];
  auto tex_uniform = glGetUniformLocation(info.program_id, name.c_str());

  if (tex_uniform >= 0)
  {
    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + idx));
    glBindTexture(GL_TEXTURE_2D, texture.internal_id());
    glUniform1i(tex_uniform, idx);
    checkErrors("glBindTexture");
  }
}

} // end namespace graphics
} // end namespace kte

