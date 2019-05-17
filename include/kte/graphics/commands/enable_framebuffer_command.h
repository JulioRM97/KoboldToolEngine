#pragma once

#include "kte/graphics/display/display_list_command.h"

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

class EnableFramebufferCommand : public DisplayListCommand
{
 public:
  EnableFramebufferCommand(const std::shared_ptr<Framebuffer>& framebuffer,
                           const CameraParameters& camera_parameters,
                           bool enable_blend,
                           ZPrePassMode z_prepass_mode = ZPrePassMode::k_Normal);

  ~EnableFramebufferCommand() override;

  void exec() const override;

 private:
  const std::shared_ptr<Framebuffer> framebuffer_;

  rect2i viewport_;

  bool enable_blend_ = false;

  ZPrePassMode z_prepass_mode_ = ZPrePassMode::k_Normal;
};

} // End namespace graphics
} // End namespace kte


