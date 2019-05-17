#pragma once

#include <cmath>

namespace kte
{
namespace math
{

template<typename T>
T LerpTo(T from, T to, T speed)
{
  if (std::abs(from - to) < speed)
  {
    return to;
  }
  if (to > from)
  {
    return from + speed;
  }
  else // to < from
  {
    return from - speed;
  }
}

} // namespace math
} // namespace kte