#pragma once

#include "kte/types.h"

#include <typeinfo>

namespace kte
{
namespace graphics
{

class DisplayListCommand
{
 public:
  DisplayListCommand();

  DisplayListCommand(const DisplayListCommand& other);

  DisplayListCommand(DisplayListCommand&& other) noexcept;

  virtual ~DisplayListCommand();

  virtual void exec() const = 0;
};

} // end namespace graphics
} // end namespace kte

