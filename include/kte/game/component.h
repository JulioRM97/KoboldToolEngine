#pragma once

#include "kte/game/updatable.h"

#include <typeinfo>

namespace kte
{

class Node;

class Component : public DefaultUpdatable
{
 public:
  explicit Component(Node& parent);

  virtual ~Component() = default;

  virtual void editorSettings() {};

  virtual const char* name() const;

  Node& owner_node_;
};

} // end namespace kte

