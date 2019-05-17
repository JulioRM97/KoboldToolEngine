#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <memory>

namespace kte
{
namespace graphics
{

class PrintDebugInformationCommand : public DisplayListCommand
{
 public:
  PrintDebugInformationCommand();

  ~PrintDebugInformationCommand() override;

  void exec() const override;
};

} // end namespace graphics
} // end namespace kte

