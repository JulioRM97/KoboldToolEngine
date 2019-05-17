#pragma once

#include "kte/graphics/display/display_list_command.h"

namespace kte
{

class Window;

namespace graphics
{

class StartFrameCommand : public DisplayListCommand
{
 public:
  StartFrameCommand();

  ~StartFrameCommand() override;

  void exec() const override;
};

} // end namespace graphics
} // end namespace kte

