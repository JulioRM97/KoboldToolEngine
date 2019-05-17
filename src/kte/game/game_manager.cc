#include "kte/game/game_manager.h"

#include "kte/audio/audio_manager.h"
#include "kte/editor/editor_manager.h"
#include "kte/game/node_manager.h"
#include "kte/game/game_systems/game_systems_manager.h"
#include "kte/game/components/camera_component.h"
#include "kte/graphics/render_manager.h"
#include "kte/graphics/window.h"
#include "kte/graphics/display/display_list_buffer.h"
#include "kte/graphics/materials/programs/metallic_roughness_material_program.h"
#include "kte/input/input_manager.h"
#include "kte/state_machine/graph_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{

////////////////////////////////////////////////////////////////
struct GameManagerData
{
  template<typename T>
  T& createUpdatable();

  kte::GameSystemsManager game_systems_manager_;

  kte::system::ThreadedScheduler scheduler_;

  kte::graphics::DisplayListBuffer display_list_buffer_;

  kte::graphics::Window window_;

  std::list<std::shared_ptr<kte::Updatable>> updatables_;
};

static GameManagerData gGameManagerData;

////////////////////////////////////////////////////////////////
template<typename T>
T& GameManagerData::createUpdatable()
{
  updatables_.push_back(std::make_shared<T>());
  return *static_cast<T*>(updatables_.back().get());
}

////////////////////////////////////////////////////////////////
GameManager::GameManager()
    : // Game singletons
      game_systems_manager_(gGameManagerData.game_systems_manager_),
      scheduler_(gGameManagerData.scheduler_),
      display_list_buffer_(gGameManagerData.display_list_buffer_),
      window_(gGameManagerData.window_),

      // Game managers
      audio_manager_(gGameManagerData.createUpdatable<kte::AudioManager>()),
      graph_manager_(gGameManagerData.createUpdatable<kte::GraphManager>()),
      input_manager_(gGameManagerData.createUpdatable<kte::input::InputManager>()),
      editor_manager_(gGameManagerData.createUpdatable<kte::EditorManager>()),
      render_manager_(gGameManagerData.createUpdatable<kte::graphics::RenderManager>()),

      // Default values
      pbr_program_(std::make_shared<kte::graphics::MetallicRoughnessMaterialProgram>()),

      // Global objects
      current_camera_(nullptr)
{
}

////////////////////////////////////////////////////////////////
bool GameManager::startup(const Game* game)
{
  if (!window_.init("Phantom Engine", 1240, 957))
  {
    return false;
  }

  current_game_ = game;

#ifdef __EMSCRIPTEN__
  scheduler_.startConsumers(0);
#else
  scheduler_.startConsumers(3);
#endif

  render_manager_.init();

  running_ = true;

  return true;
}

////////////////////////////////////////////////////////////////
void GameManager::shutdown()
{
  running_ = false;

  gGameManagerData.updatables_.clear();
  window_.finish();
  current_game_ = nullptr;
}

////////////////////////////////////////////////////////////////
const std::list<std::shared_ptr<kte::Updatable>>& GameManager::updatables() const
{
  return gGameManagerData.updatables_;
}

}
