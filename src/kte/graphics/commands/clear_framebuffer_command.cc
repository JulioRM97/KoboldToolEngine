#include "kte/graphics/commands/clear_framebuffer_command.h"

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
ClearFramebufferCommand::ClearFramebufferCommand(const std::shared_ptr<Framebuffer> framebuffer)
    : framebuffer_(framebuffer)
{
}

////////////////////////////////////////////////////////////////
ClearFramebufferCommand::ClearFramebufferCommand(const std::shared_ptr<Framebuffer> framebuffer,
                                                 glm::vec4 color)
    : framebuffer_(framebuffer),
      color_(color)
{
}

////////////////////////////////////////////////////////////////
ClearFramebufferCommand::~ClearFramebufferCommand() = default;

////////////////////////////////////////////////////////////////
void ClearFramebufferCommand::exec() const
{
  framebuffer_->clear(color_);
}

} // end namespace graphics
} // end namespace kte

