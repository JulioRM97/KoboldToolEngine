#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <deque>
#include <functional>
#include <memory>

namespace kte
{
namespace graphics
{

class DrawEditorCommand : public DisplayListCommand
{
 public:
  DrawEditorCommand();

  DrawEditorCommand(const DrawEditorCommand& other);

  DrawEditorCommand(DrawEditorCommand&& other) noexcept;

  ~DrawEditorCommand() override;

  void exec() const override;

};

} // namespace kte
} // namespace graphics

