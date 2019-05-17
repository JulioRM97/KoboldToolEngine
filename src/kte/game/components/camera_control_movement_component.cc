#include "kte/game/components/camera_control_movement_component.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "kte/timer.h"
#include "kte/game/game_manager.h"
#include "kte/game/node.h"
#include "kte/game/components/camera_component.h"
#include "kte/graphics/window.h"
#include "kte/input/input_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{

static const char* translate_input_name = "TranslateCameraMovement";
static const char* rotate_key_input_name = "RotateKeyCameraMovement";
static const char* rotate_mouse_button_input_name = "RotateMouseButtonCameraMovement";
static const char* rotate_mouse_input_name = "RotateMouseCameraMovement";

////////////////////////////////////////////////////////////////
CameraControlMovementComponent::CameraControlMovementComponent(Node& parent)
    : Component(parent)
{
}

////////////////////////////////////////////////////////////////
CameraControlMovementComponent::~CameraControlMovementComponent() = default;

////////////////////////////////////////////////////////////////
void CameraControlMovementComponent::update()
{
  const auto& input_manager = GameManager::Instance().input_manager_;
  const auto* camera = owner_node_.getComponent<CameraComponent>();

  auto* transform = owner_node_.transform();

  // Translate
  const auto translate = input_manager.axes_.at(translate_input_name).value * camera->view();

  const auto delta_position = Time::deltaTime * cam_speed_ * glm::vec3(translate);
  const auto new_translation = transform->local_translation() + delta_position;
  transform->set_local_translation(new_translation);

  // Rotate
  const auto rotate_key = input_manager.axes_.at(rotate_key_input_name).value;
  auto delta_rotation = Time::deltaTime * rotate_speed_ * rotate_key;

  bool mouse_button_pressed = input_manager.buttons_.at(rotate_mouse_button_input_name).pressed;
  if (mouse_button_pressed)
  {
    const auto rotate_mouse = input_manager.axes_.at(rotate_mouse_input_name).value;
    delta_rotation += Time::deltaTime * rotate_speed_ * rotate_mouse;
  }

  GameManager::Instance().scheduler_.schedule([mouse_button_pressed]() {
    GameManager::Instance().window_.captureMouse(mouse_button_pressed);
  }, true);

  pitch_ += delta_rotation.y;
  yaw_ += delta_rotation.x;

  transform->set_local_rotation(glm::angleAxis(yaw_, glm::vec3(0, 1, 0)) *
                                glm::angleAxis(pitch_, glm::vec3(1, 0, 0)));
}

////////////////////////////////////////////////////////////////
void CameraControlMovementComponent::registerDefaultInput()
{
  auto& input_manager = kte::GameManager::Instance().input_manager_;

  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_A, -1.0f, X);
  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_D, 1.0f, X);
  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_Q, -1.0f, Y);
  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_E, 1.0f, Y);
  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_W, -1.0f, Z);
  input_manager.addAxisKeyInput(translate_input_name, GLFW_KEY_S, 1.0f, Z);

  input_manager.addAxisKeyInput(rotate_key_input_name, GLFW_KEY_LEFT, 1.0f, X);
  input_manager.addAxisKeyInput(rotate_key_input_name, GLFW_KEY_RIGHT, -1.0f, X);
  input_manager.addAxisKeyInput(rotate_key_input_name, GLFW_KEY_UP, -1.0f, Y);
  input_manager.addAxisKeyInput(rotate_key_input_name, GLFW_KEY_DOWN, 1.0f, Y);

  input_manager.addButtonMouseInput(rotate_mouse_button_input_name, GLFW_MOUSE_BUTTON_RIGHT);

  input_manager.addAxisMouseInput(rotate_mouse_input_name, X, -0.25f, X);
  input_manager.addAxisMouseInput(rotate_mouse_input_name, Y, -0.25f, Y);
}

} // end namespace kte

