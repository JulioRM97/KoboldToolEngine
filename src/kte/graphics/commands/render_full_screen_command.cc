#include "kte/graphics/commands/render_full_screen_command.h"

#include <glm/gtc/type_ptr.hpp>

#include "kte/graphics/gl_internals.h"

#include "kte/game/components/light_component.h"
#include "kte/game/game_manager.h"
#include "kte/game/components/camera_component.h"
#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
static const float kPositions[] = {
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
};

////////////////////////////////////////////////////////////////
static const float kTexCoords[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

////////////////////////////////////////////////////////////////
RenderFullScreenCommand::RenderFullScreenCommand(const std::shared_ptr<FullScreenProgram> program,
                                                 const std::shared_ptr<Framebuffer> input,
                                                 const std::shared_ptr<Framebuffer> output,
                                                 const LightComponent* light)
    : DisplayListCommand(),
      program_(program),
      input_(input),
      output_(output),
      light_(light)
{
}

////////////////////////////////////////////////////////////////
RenderFullScreenCommand::~RenderFullScreenCommand() = default;

////////////////////////////////////////////////////////////////
void RenderFullScreenCommand::exec() const
{
  // FIXME this could be a memory leak
  static GLuint vao_id = 0;

  if (!vao_id)
  {
    glGenVertexArrays(1, &vao_id);

    glBindVertexArray(vao_id);
    checkErrors("glBindVertexArray");

    GLuint id = 0;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kPositions), &kPositions, GL_STATIC_DRAW);
    checkErrors("glBufferData kPositions");

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    checkErrors("glVertexAttribPointer kPositions");

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kTexCoords), &kTexCoords, GL_STATIC_DRAW);
    checkErrors("glBufferData kTexCoords");

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, nullptr);
    checkErrors("glVertexAttribPointer kTexCoords");
  }

  program_->load();
  program_->setupInputFramebuffer(*input_, RenderMode::k_FullScreen);

  output_->use();

  const auto& info = program_->data_->programs[RenderMode::k_FullScreen];

  if (light_)
  {
    light_->use(info);
  }
  checkErrors("clean");


  glUseProgram(info.program_id);
  checkErrors("glUseProgram");

  GLint proj_id = info.uniforms.at("PROJECTION");
  if (proj_id >= 0)
  {
    glUniformMatrix4fv(proj_id, 1, GL_FALSE, glm::value_ptr(GameManager::Instance().current_camera_->projection()));
    checkErrors("glUniformMatrix4fv PROJECTION");
  }

  GLint noise_id = info.uniforms.at("noise");
  if (noise_id >= 0)
  {
    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_2D, program_->noise_tex_->internal_id());
    glUniform1i(noise_id, 30);
    checkErrors("glUniform1f noise tex");
  }

  GLint samples_id = info.uniforms.at("samples");
  if (samples_id >= 0)
  {
    glUniform3fv(samples_id, 64, &program_->ssao_kernel_.data()->x);
  }

  glBindVertexArray(vao_id);
  checkErrors("glBindVertexArray");

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  checkErrors("glDrawArrays");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);

  checkErrors("RenderFullScreenCommand::exec DONE");
}

} // end namespace graphics
} // end namespace kte

