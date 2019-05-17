#pragma once

#include <memory>
#include <list>

#include "kte/game/updatable.h"

// Forward declarations
namespace kte
{

class AudioManager;
class CameraComponent;
class Game;
class GameSystemsManager;
class GraphManager;
class EditorManager;
class Node;

namespace input
{
class InputManager;
} // namespace input

namespace graphics
{
class DisplayListBuffer;
class Window;
class Material;
class Program;
class RenderManager;
} // namespace graphics

namespace system
{
class ThreadedScheduler;
} // namespace system

} // namespace kte

namespace kte
{

/// The main game manager. Contains all managers and global state.
class GameManager
{
 public:
  static inline GameManager& Instance()
  {
    static GameManager instance;
    return instance;
  }

  bool startup(const Game* game);

  void shutdown();

  const std::list<std::shared_ptr<kte::Updatable>>& updatables() const;

  bool running_ = false;

  // Game singletons

  kte::GameSystemsManager& game_systems_manager_;

  kte::system::ThreadedScheduler& scheduler_;

  kte::graphics::DisplayListBuffer& display_list_buffer_;

  kte::graphics::Window& window_;

  // Game managers

  kte::AudioManager& audio_manager_;

  kte::GraphManager& graph_manager_;

  kte::input::InputManager& input_manager_;

  kte::EditorManager& editor_manager_;

  kte::graphics::RenderManager& render_manager_;

  // Default values

  std::shared_ptr<kte::graphics::Program> pbr_program_;

  // Global objects

  const kte::Game* current_game_ = nullptr;

  kte::CameraComponent* current_camera_ = nullptr;

 private:
  GameManager();
};

} // namespace kte

