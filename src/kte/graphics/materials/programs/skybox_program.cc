#include "skybox_program.h"
#include "kte/graphics/gl_internals.h"

#include <glm/gtc/type_ptr.hpp>

#include "kte/game/components/light_component.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
SkyboxProgram::SkyboxProgram()
    : Program(new GPUData())
{
  data_->base_shader_name = "skybox";

  data_->required_uniforms = {{"PROJECTION", "u_m_projection"},
                              {"VIEW", "u_m_view"}};

  data_->programs[RenderMode::k_Forward];
  data_->programs[RenderMode::k_Deferred];
  data_->programs[RenderMode::k_ZPrePass];
  data_->programs[RenderMode::k_ShadowMap];

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
SkyboxProgram::~SkyboxProgram() = default;

////////////////////////////////////////////////////////////////
void SkyboxProgram::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void SkyboxProgram::draw(const Drawable& drawable,
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
