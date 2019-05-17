#include <kte/graphics/window.h>
#include "kte/editor/draw_editor_command.h"

#include "kte/editor/editor_manager.h"
#include "kte/game/game_manager.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////
DrawEditorCommand::DrawEditorCommand() = default;

/////////////////////////////////////////////
DrawEditorCommand::~DrawEditorCommand() = default;

/////////////////////////////////////////////
DrawEditorCommand::DrawEditorCommand(const DrawEditorCommand& other) = default;

/////////////////////////////////////////////
DrawEditorCommand::DrawEditorCommand(DrawEditorCommand&& other) noexcept = default;

/////////////////////////////////////////////
void DrawEditorCommand::exec() const
{
  if (kte::GameManager::Instance().window_.enable_imgui_)
  {
    kte::GameManager::Instance().editor_manager_.draw();
  }
}

} // namespace graphics
} // namespace kte
