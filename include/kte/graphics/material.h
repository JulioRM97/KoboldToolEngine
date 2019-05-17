#pragma once

#include <memory>

#include "kte/types.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class Program;

class Material
{
 public:
  virtual void setupProgram(RenderMode mode) = 0;

  std::shared_ptr<Program> program_;

 protected:
  virtual ~Material() = default;

};

} // end namespace graphics
} // end namespace kte

