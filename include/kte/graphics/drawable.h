#pragma once

#include <memory>

namespace kte
{
namespace graphics
{

class Program;

class Drawable
{
 public:
  virtual void load() = 0;

  virtual void draw() const = 0;

  virtual void unload() = 0;

  virtual Program& program() const = 0;
};

} // end namespace graphics
} // end namespace kte

