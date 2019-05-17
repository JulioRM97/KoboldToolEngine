#pragma once

#include "kte/game/updatable.h"

#include <cstdio>
#include <typeinfo>

#include "kte/system/memory_manager.hpp"

namespace kte
{

class Component;

////////////////////////////////////////////////////////////////
template<typename T>
class GameSystem : public DefaultUpdatable
{
 public:
  void preUpdate() override;

  void update() override;

  void fixedUpdate() override;

  void postUpdate() override;

  MemoryManager<T> memory_manager_;
};

////////////////////////////////////////////////////////////////
template<typename T>
void GameSystem<T>::preUpdate()
{
  for (auto& component : memory_manager_)
  {
    component.preUpdate();
  }
}

////////////////////////////////////////////////////////////////
template<typename T>
void GameSystem<T>::update()
{
  for (auto& component : memory_manager_)
  {
    component.update();
  }
}

////////////////////////////////////////////////////////////////
template<typename T>
void GameSystem<T>::fixedUpdate()
{
  for (auto& component : memory_manager_)
  {
    component.fixedUpdate();
  }
}

////////////////////////////////////////////////////////////////
template<typename T>
void GameSystem<T>::postUpdate()
{
  for (auto& component : memory_manager_)
  {
    component.postUpdate();
  }
}

} // end namespace kte

