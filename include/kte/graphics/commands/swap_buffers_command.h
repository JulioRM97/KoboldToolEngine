#pragma once

#include "kte/graphics/display/display_list_command.h"

namespace kte
{
namespace graphics
{

class SwapBuffersCommand : public DisplayListCommand
{
 public:
  SwapBuffersCommand();

  ~SwapBuffersCommand() override;

  void exec() const override;
};

} // end namespace graphics
} // end namespace kte

