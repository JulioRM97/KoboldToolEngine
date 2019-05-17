#include "kte/graphics/commands/enable_framebuffer_command.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
EnableFramebufferCommand::EnableFramebufferCommand(const std::shared_ptr<Framebuffer>& framebuffer,
                                                   const CameraParameters& camera_parameters,
                                                   bool enable_blend,
                                                   ZPrePassMode z_prepass_mode)
    : framebuffer_(framebuffer),
      viewport_(camera_parameters.viewport()),
      enable_blend_(enable_blend),
      z_prepass_mode_(z_prepass_mode)
{
}

////////////////////////////////////////////////////////////////
EnableFramebufferCommand::~EnableFramebufferCommand() = default;

////////////////////////////////////////////////////////////////
void EnableFramebufferCommand::exec() const
{
  framebuffer_->use(z_prepass_mode_);
  framebuffer_->enableViewport(viewport_);
  framebuffer_->toggleBlend(enable_blend_);
}

} // End namespace graphics
} // End namespace kte

