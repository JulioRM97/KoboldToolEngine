#include "kte/game/game.h"
#include "kte/common.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "kte/game/game_manager.h"
#include "kte/game/game_systems/game_systems_manager.h"
#include "kte/game/scene.h"
#include "kte/game/node.h"
#include "kte/game/components/camera_component.h"
#include "kte/game/components/transform_component.h"
#include "kte/game/components/light_component.h"
#include "kte/graphics/window.h"
#include "kte/graphics/display/display_list_buffer.h"
#include "kte/system/threaded_scheduler.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
Game::Game() = default;

////////////////////////////////////////////////////////////////
Game::~Game() = default;

////////////////////////////////////////////////////////////////
void Game::addMissingRequiredComponents()
{
  if (addedMissingRequiredComponents)
  {
    return;
  }
  addedMissingRequiredComponents = true;

  if (scenes_.empty())
  {
    scenes_.emplace_back(new Scene());
    current_scene_id_ = 0;
  }

  for (auto& scene : scenes_)
  {
    auto camera = GameManager::Instance().current_camera_;

    if (!camera)
    {
      auto& node = scene->addChild();
      node.transform()->set_local_translation(glm::vec3());

      camera = node.createComponent<CameraComponent>();
      camera->set_perspective(2.0f / 3.0f, 1.5f, 0.1f, 100.0f);
      GameManager::Instance().current_camera_ = camera;
    }

    auto& emissive_light_node = scene->addChild();
    auto emissive_light = emissive_light_node.createComponent<LightComponent>();
    emissive_light->set_type(LightType::k_Emissive);
  }
}

#ifdef __EMSCRIPTEN__

////////////////////////////////////////////////////////////////
static void game_main_loop(void* game)
{
  static_cast<Game*>(game)->main_loop();
}

#endif

////////////////////////////////////////////////////////////////
void Game::start()
{
  mtr_init("mtr_output.json");
  $F

#ifdef __EMSCRIPTEN__
  // There's a check on emscripten code that requires a main loop function set
  // to initialize the window
  emscripten_set_main_loop_arg(&game_main_loop, this, -1, 0);
#endif

  addMissingRequiredComponents();
  kte::GameManager::Instance().startup(this);
  frame_timer_.start();

#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
  emscripten_set_main_loop_arg(&game_main_loop, this, -1, 1);
#else
  while (main_loop());
#endif
}

////////////////////////////////////////////////////////////////
bool Game::main_loop()
{
  auto& manager = kte::GameManager::Instance();
  auto& scheduler = manager.scheduler_;

  manager.running_ = manager.window_.processEvents();

  // Sync and wait for previous frame tasks to finish
  scheduler.flush();

  // From this point onward, send new commands to a new display list
  manager.display_list_buffer_.swap();

  // Calculate the delta time.
  Time::deltaTime = frame_timer_.elapsed(true);

  // Process this frame logic and the previous frame painting in parallel.
  scheduler.schedule(std::bind(&Game::update, this));
  manager.display_list_buffer_.flush();

  if (!manager.running_)
  {
    shutdown();
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////
void Game::update() const
{
  kte::GameManager::Instance().game_systems_manager_.runFrame(*this);
}

////////////////////////////////////////////////////////////////
const Scene* Game::current_scene() const
{
  if (current_scene_id_ >= 0 && (size_t) current_scene_id_ < scenes_.size())
  {
    return scenes_[current_scene_id_].get();
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////
bool Game::shutdown()
{
  auto& manager = kte::GameManager::Instance();
  auto& scheduler = manager.scheduler_;

  scheduler.stopConsumers();

  mtr_flush();
  mtr_shutdown();

  manager.shutdown();

  return true;
}

} // end namespace kte

