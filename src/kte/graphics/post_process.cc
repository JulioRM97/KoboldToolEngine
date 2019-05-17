#include "kte/graphics/post_process.h"
#include "kte/graphics/gl_internals.h"

#include "kte/graphics/commands/enable_framebuffer_command.h"
#include "kte/graphics/commands/clear_framebuffer_command.h"
#include "kte/graphics/commands/render_full_screen_command.h"
#include "kte/graphics/display/display_list.h"
#include "kte/graphics/postprocess/post_process_program.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
PostProcess::PostProcess(std::initializer_list<std::string> names)
{
  passes_.resize(names.size());

  uint8_t idx = 0;
  for (auto& name : names)
  {
    passes_[idx].program = std::make_shared<PostProcessProgram>(name);
    passes_[idx].input_framebuffer = std::make_shared<Framebuffer>();
    idx++;
  }
}

////////////////////////////////////////////////////////////////
PostProcess::~PostProcess() = default;

////////////////////////////////////////////////////////////////
std::shared_ptr<Framebuffer> PostProcess::input_framebuffer() const
{
  return passes_.at(0).input_framebuffer;
}

////////////////////////////////////////////////////////////////
void PostProcess::render(const std::shared_ptr<Framebuffer> output,
                         const LightComponent* light,
                         DisplayList& list) const
{
  uint8_t idx = 0;
  for (const auto& pass : passes_)
  {
    idx++;
    const auto& fb = idx == passes_.size() ? output : passes_[idx].input_framebuffer;
    renderPass(fb, light, pass, list);
  }
}

////////////////////////////////////////////////////////////////
void PostProcess::renderPass(const std::shared_ptr<Framebuffer> output,
                             const LightComponent* light,
                             const PostProcessPass& pass,
                             DisplayList& list) const
{
  list.add<ClearFramebufferCommand>(output);
  list.add<RenderFullScreenCommand>(pass.program, pass.input_framebuffer, output, light);
}

////////////////////////////////////////////////////////////////
void PostProcess::set_screen_size(glm::ivec2 screen_size)
{
  for (auto& pass : passes_)
  {
    pass.input_framebuffer->set_size(screen_size);
  }
}

} // end namespace graphics
} // end namespace kte

