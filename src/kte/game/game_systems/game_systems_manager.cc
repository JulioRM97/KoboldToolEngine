#include "kte/game/game_systems/game_systems_manager.h"
#include "kte/common.h"

#include "kte/timer.h"
#include "kte/audio/audio_manager.h"
#include "kte/game/game_manager.h"
#include "kte/game/game.h"
#include "kte/game/scene.h"
#include "kte/game/node.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{

////////////////////////////////////////////////////////////////
void GameSystemsManager::runFrame(const Game& game)
{
  auto* scene = game.current_scene();
  if (!scene)
  {
    return;
  }

  auto& manager = GameManager::Instance();
  auto& scheduler = manager.scheduler_;
  const auto updatables = manager.updatables();

  // Run pre update in sequence
  {
    scheduler.schedule(std::bind(&GameSystemsManager::runScenePreUpdate,
                                 this, scene)).sync();

    for (const auto& updatable : updatables)
    {
      scheduler.schedule(std::bind(&Updatable::preUpdate, updatable.get()),
                         true).sync();
    }
  }

  // Run fixed updates in parallel
  {
    const auto dt = Time::fixedDeltaTime;
    Time::elapsedTime += Time::deltaTime;
    Time::remainingSimulationTime += Time::deltaTime;
    const auto count = (uint32_t) (Time::remainingSimulationTime / dt);
    Time::remainingSimulationTime -= count * dt;

    scheduler.schedule(std::bind(&GameSystemsManager::runSceneFixedUpdate,
                                 this, scene, updatables, count));
  }

  // Tell the scheduler to wait for all the fixed updates to finish
  scheduler.sync();

  // Run updates in parallel
  {
    scheduler.schedule(std::bind(&GameSystemsManager::runSceneUpdate,
                                 this, scene));

    for (const auto& updatable : updatables)
    {
      scheduler.schedule(std::bind(&Updatable::update, updatable.get()), true);
    }
  }

  // Tell the scheduler to wait for all the updates to finish
  scheduler.sync();

  // Run post update in sequence
  {
    scheduler.schedule(std::bind(&GameSystemsManager::runScenePostUpdate,
                                 this, scene)).sync();

    for (const auto& updatable : updatables)
    {
      scheduler.schedule(std::bind(&Updatable::postUpdate, updatable.get()),
                         true).sync();
    }
  }
}

////////////////////////////////////////////////////////////////
void GameSystemsManager::runScenePreUpdate(const Scene* scene)
{ $
  for (const auto& game_system : scene->game_systems_)
  {
    game_system.second->preUpdate();
  }
}

////////////////////////////////////////////////////////////////
void GameSystemsManager::runSceneFixedUpdate(const Scene* scene,
                                             const std::list<std::shared_ptr<kte::Updatable>>& updatables,
                                             uint32_t count)
{ $
  auto& scheduler = GameManager::Instance().scheduler_;

  for (uint32_t i = 0; i < count; i++)
  {
    for (const auto& pair : scene->game_systems_)
    {
      auto game_system = pair.second;
      scheduler.schedule(std::bind(&Updatable::fixedUpdate, game_system.get()));
    }

    for (const auto& updatable : updatables)
    {
      scheduler.schedule(std::bind(&Updatable::fixedUpdate, updatable.get()),
                         true);
    }

    // Sync every cycle of simulation
    scheduler.sync();
  }
}

////////////////////////////////////////////////////////////////
void GameSystemsManager::runSceneUpdate(const Scene* scene)
{ $
  auto& scheduler = GameManager::Instance().scheduler_;

  for (const auto& pair : scene->game_systems_)
  {
    auto game_system = pair.second;
    scheduler.schedule(std::bind(&Updatable::update, game_system.get()));
  }
}

////////////////////////////////////////////////////////////////
void GameSystemsManager::runScenePostUpdate(const Scene* scene)
{ $
  for (const auto& game_system : scene->game_systems_)
  {
    game_system.second->postUpdate();
  }
}

} // end namespace kte
