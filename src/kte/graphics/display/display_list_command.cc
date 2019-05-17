#include "kte/graphics/display/display_list_command.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
DisplayListCommand::DisplayListCommand() = default;

////////////////////////////////////////////////////////////////
DisplayListCommand::DisplayListCommand(const DisplayListCommand& other) = default;

////////////////////////////////////////////////////////////////
DisplayListCommand::DisplayListCommand(DisplayListCommand&& other) noexcept = default;

////////////////////////////////////////////////////////////////
DisplayListCommand::~DisplayListCommand() = default;

}//end namespace commands
}//end namespace graphics
