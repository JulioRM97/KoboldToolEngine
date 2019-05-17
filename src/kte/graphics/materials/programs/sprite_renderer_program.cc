#include "sprite_renderer_program.h"
#include "kte/graphics/gl_internals.h"

#include <glm/gtc/type_ptr.hpp>

#include "kte/game/components/light_component.h"
#include "kte/graphics/drawables/sprite.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
SpriteRendererProgram::SpriteRendererProgram()
{
  data_->base_shader_name = "sprite_renderer";

  data_->required_uniforms = {{"PROJECTION", "u_m_projection"},
                              {"VIEW", "u_m_view"},
                              {"MODEL", "u_m_model"},
                              {"CENTER", "u_v_center"},
                              {"SCALE", "u_v_scale"}};

  data_->programs[RenderMode::k_Forward];
  data_->programs[RenderMode::k_Deferred];
  data_->programs[RenderMode::k_ZPrePass];
  data_->programs[RenderMode::k_ShadowMap];

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
SpriteRendererProgram::~SpriteRendererProgram() = default;

////////////////////////////////////////////////////////////////
void SpriteRendererProgram::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void SpriteRendererProgram::draw(const Drawable& drawable,
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
    checkErrors("glUniformMatrix4fv MODEL");
  }

  GLint proj_id = info.uniforms.at("PROJECTION");
  if (proj_id >= 0)
  {
    glUniformMatrix4fv(proj_id, 1, GL_FALSE, glm::value_ptr(projection));
    checkErrors("glUniformMatrix4fv PROJECTION");
  }

  GLint view_id = info.uniforms.at("VIEW");
  if (view_id >= 0)
  {
    glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
    checkErrors("glUniformMatrix4fv VIEW");
  }

  drawable.draw();

  glUseProgram(0);
}

////////////////////////////////////////////////////////////////
void SpriteRendererProgram::applySpriteParams(const Sprite& sprite) const
{
  // TODO use correct mode
  const auto& info = data_->programs[RenderMode::k_Forward];

  GLint center_id = info.uniforms.at("CENTER");
  if (center_id >= 0)
  {
    glUniform2fv(center_id, 1, glm::value_ptr(sprite.center_));
    checkErrors("glUniform2fv u_v_center");
  }

  GLint scale_id = info.uniforms.at("SCALE");
  if (scale_id >= 0)
  {
    auto scale = sprite.texture_scale_ * glm::vec2(sprite.texture_->size());
    glUniform2fv(scale_id, 1, glm::value_ptr(scale));
    checkErrors("glUniform2fv u_v_scale");
  }
}

} // end namespace graphics
} // end namespace kte
