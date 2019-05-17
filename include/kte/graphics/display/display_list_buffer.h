#pragma once

#include <mutex>
#include <vector>

#include "display_list.h"

namespace kte
{
namespace graphics
{

/// A "double-buffered" display list queue.
class DisplayListBuffer
{
 public:
  std::shared_ptr<kte::graphics::DisplayList> createList() const;

  /// Submit a new list that eventually will be executed.
  void submit(std::shared_ptr<kte::graphics::DisplayList> display_list);

  /// Swap the read and write list.
  void swap();

  /// Execute the pending commands from the read list.
  void flush();

 private:
  std::vector<std::shared_ptr<kte::graphics::DisplayList>> display_lists_front_;

  std::vector<std::shared_ptr<kte::graphics::DisplayList>> display_lists_back_;

  std::mutex write_mutex_;
};

} // end namespace graphics
} // end namespace kte

