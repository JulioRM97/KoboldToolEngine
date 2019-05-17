#include "kte/game/component.h"

#ifndef MSVC
#include <cxxabi.h>
#endif

namespace kte
{

////////////////////////////////////////////////////////////////
Component::Component(Node& parent)
    : owner_node_(parent)
{
}

////////////////////////////////////////////////////////////////
const char* Component::name() const
{
#ifndef MSVC
  return abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, nullptr);
#else
  return typeid(*this).name();
#endif
}

} // end namespace kte

