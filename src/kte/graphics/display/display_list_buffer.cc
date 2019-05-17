#include "kte/graphics/display/display_list_buffer.h"
#include "kte/common.h"

#include "kte/game/game_manager.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
std::shared_ptr<kte::graphics::DisplayList> DisplayListBuffer::createList() const
{$
  return std::make_shared<DisplayList>();
}

////////////////////////////////////////////////////////////////
void DisplayListBuffer::submit(std::shared_ptr<kte::graphics::DisplayList> display_list)
{$
  std::lock_guard<std::mutex> guard(write_mutex_);
  display_lists_back_.push_back(display_list);
}

////////////////////////////////////////////////////////////////
void DisplayListBuffer::swap()
{$
  std::lock_guard<std::mutex> guard(write_mutex_);
  std::swap(display_lists_front_, display_lists_back_);
}

////////////////////////////////////////////////////////////////
void DisplayListBuffer::flush()
{$
  for (const auto& list : display_lists_front_)
  {
    list->execute();
  }
  display_lists_front_.clear();
}

} // end namespace graphics
} // end namespace kte

