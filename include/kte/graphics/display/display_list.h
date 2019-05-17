#pragma once

#include <deque>
#include <memory>

#include "kte/graphics/display/display_list_command.h"

namespace kte
{
namespace graphics
{

class DisplayList
{
 public:
  DisplayList();

  DisplayList(const DisplayList& other);

  ~DisplayList();

  void execute();

  /// Add a command from a pointer reference
  template<typename T>
  inline void add(T*&& command) { add(std::shared_ptr<T>(command)); }

  /// Create and add a command from a template
  template<typename T, typename... Args>
  inline void add(Args&& ... args)
  {
    add(std::make_shared<T>(std::forward<Args>(args)...));
  }

  /// Add a command
  void add(std::shared_ptr<DisplayListCommand> command);

  std::deque<std::shared_ptr<DisplayListCommand>> commands_;
};

} // end namespace graphics
} // end namespace kte

