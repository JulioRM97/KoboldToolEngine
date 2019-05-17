#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <glm/vec4.hpp>

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

class ClearFramebufferCommand : public DisplayListCommand
{
 public:
  explicit ClearFramebufferCommand(std::shared_ptr<Framebuffer> framebuffer);

  ClearFramebufferCommand(std::shared_ptr<Framebuffer> framebuffer,
                          glm::vec4 color);

  ~ClearFramebufferCommand() override;

  void exec() const override;

 private:
  std::shared_ptr<Framebuffer> framebuffer_;

  glm::vec4 color_{0.0f, 0.0f, 0.0f, 0.0f};

};

} // end namespace graphics
} // end namespace kte

