#pragma once

#include <deque>
#include <memory>

#include "kte/timer.h"

namespace kte
{

class Scene;

class Game
{
 public:
  Game();

  ~Game();

  void addMissingRequiredComponents();

  void start();

  bool main_loop();

  void update() const;

  const Scene* current_scene() const;

  std::deque<std::shared_ptr<Scene>> scenes_;

  int current_scene_id_ = -1;

 private:
  bool shutdown();

  Timer frame_timer_;

  bool addedMissingRequiredComponents = false;
};

} // end namespace kte

