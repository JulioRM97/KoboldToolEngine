#pragma once

#include <array>
#include <list>

#include <glm/vec4.hpp>

#include "kte/types.h"

namespace kte
{
namespace input
{

struct AxisKeyInput
{
  AxisKeyInput(Key key, float value, int dimension = 0)
      : key(key),
        value(value),
        dimension(dimension) {};

  Key key;

  float value = 1.0;

  int dimension = 0;
};

struct AxisMouseInput
{
  AxisMouseInput(int axis, float value, int dimension = 0)
      : axis(axis),
        value(value),
        dimension(dimension) {};

  int axis;

  float value = 1.0;

  int dimension = 0;
};

struct Axis
{
  std::list<AxisKeyInput> key_inputs;
  std::list<AxisMouseInput> mouse_inputs;

  glm::vec4 value;
};

} // namespace input
} // namespace kte

