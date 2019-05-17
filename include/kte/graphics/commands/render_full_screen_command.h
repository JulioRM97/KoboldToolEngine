#pragma once

#include <memory>

#include "kte/graphics/display/display_list_command.h"

namespace kte
{

class LightComponent;

namespace graphics
{

// Forward declarations
class FullScreenProgram;
class Framebuffer;

class RenderFullScreenCommand : public DisplayListCommand
{
 public:
  RenderFullScreenCommand(std::shared_ptr<FullScreenProgram> program,
                          std::shared_ptr<Framebuffer> input,
                          std::shared_ptr<Framebuffer> output,
                          const LightComponent* light);

  ~RenderFullScreenCommand() override;

  void exec() const override;

 private:
  std::shared_ptr<FullScreenProgram> program_;

  std::shared_ptr<Framebuffer> input_;

  std::shared_ptr<Framebuffer> output_;

  const LightComponent* light_;

};

} // end namespace graphics
} // end namespace kte

