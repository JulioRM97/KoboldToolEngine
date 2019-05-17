#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <glm/vec4.hpp>

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

class CopyFramebuffersCommand : public DisplayListCommand
{
 public:
  CopyFramebuffersCommand(std::shared_ptr<Framebuffer> framebuffer_from,
                          std::shared_ptr<Framebuffer> framebuffer_to);

  ~CopyFramebuffersCommand() override;

  void exec() const override;

 private:
  std::shared_ptr<Framebuffer> framebuffer_from_;

  std::shared_ptr<Framebuffer> framebuffer_to_;

};

} // end namespace graphics
} // end namespace kte

