#pragma once

#include <list>
#include <memory>

#include "kte/game/game_systems/game_system.hpp"

namespace kte
{

class Game;
class Scene;
class Node;
class Component;

class GameSystemsManager
{
 public:
  void runFrame(const Game& game);

  /// Run all systems in sequence
  void runScenePreUpdate(const Scene* scene);

  /// Run all systems and all updatables in parallel but waits each execution
  /// for all to finish
  void runSceneFixedUpdate(const Scene* scene,
                           const std::list<std::shared_ptr<kte::Updatable>>& updatables,
                           uint32_t count);

  /// Run all systems in parallel
  void runSceneUpdate(const Scene* scene);

  /// Run all systems in sequence
  void runScenePostUpdate(const Scene* scene);
};

} // end namespace kte

