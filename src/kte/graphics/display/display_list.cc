#include "kte/graphics/display/display_list.h"
#include "kte/common.h"

#include <thread>

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
DisplayList::DisplayList() = default;

////////////////////////////////////////////////////////////////
DisplayList::DisplayList(const DisplayList& other) = default;

////////////////////////////////////////////////////////////////
DisplayList::~DisplayList() = default;

////////////////////////////////////////////////////////////////
void DisplayList::execute()
{
  for (auto& command : commands_)
  {
    //$N(command->name())
    command->exec();
  }
  commands_.clear();
}

////////////////////////////////////////////////////////////////
void DisplayList::add(std::shared_ptr<DisplayListCommand> command)
{
  commands_.push_back(command);
}

} // end namespace graphics
} // end namespace kte

