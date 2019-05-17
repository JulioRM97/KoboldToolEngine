#include "kte/input/input_manager.h"

#include <GLFW/glfw3.h>

#include "kte/game/game_manager.h"
#include "kte/graphics/window.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace input
{

////////////////////////////////////////////////////////////
InputManager::InputManager() = default;

////////////////////////////////////////////////////////////
InputManager::~InputManager() = default;

////////////////////////////////////////////////////////////
void InputManager::addButtonKeyInput(std::string name, Key key)
{
  buttons_[name].key_inputs.emplace_back(key);
}

////////////////////////////////////////////////////////////
void InputManager::addButtonMouseInput(std::string name, uint32_t button)
{
  buttons_[name].mouse_inputs.emplace_back(button);
}

////////////////////////////////////////////////////////////
void InputManager::addAxisKeyInput(std::string name,
                                   Key key,
                                   float value,
                                   int dimension)
{
  axes_[name].key_inputs.emplace_back(key, value, dimension);
}

////////////////////////////////////////////////////////////
void InputManager::addAxisMouseInput(std::string name,
                                     int axis,
                                     float value,
                                     int dimension)
{
  axes_[name].mouse_inputs.emplace_back(axis, value, dimension);
}

////////////////////////////////////////////////////////////
void InputManager::update()
{
  any_button_up_ = false;
  for (auto& button : buttons_)
  {
    readInput(button.second);
  }

  any_axis_ = false;
  for (auto& axis : axes_)
  {
    readInput(axis.second);
  }
}

////////////////////////////////////////////////////////////
void InputManager::readInput(Action& action)
{
  auto& window = GameManager::Instance().window_;

  action.pressed = false;

  for (auto& key_input : action.key_inputs)
  {
    action.pressed |= window.keyPressed(key_input.key);
  }

  for (auto& mouse_input : action.mouse_inputs)
  {
    action.pressed |= window.mouseButtonPressed(mouse_input.button);
  }

  action.down = !action.previous & action.pressed;
  action.up = action.previous & !action.pressed;
  action.previous = action.pressed;
  any_button_up_ |= action.up;
}

////////////////////////////////////////////////////////////
void InputManager::readInput(Axis& axis)
{
  auto& window = GameManager::Instance().window_;

  axis.value *= 0.0f;

  for (auto& key_input : axis.key_inputs)
  {
    if (window.keyPressed(key_input.key))
    {
      axis.value[key_input.dimension] += key_input.value;
    }

    any_axis_ |= axis.value[key_input.dimension] != 0;
  }

  for (auto& mouse_input : axis.mouse_inputs)
  {
    axis.value[mouse_input.dimension] += mouse_input.value * window.mouseAxisValue(mouse_input.axis);

    any_axis_ |= axis.value[mouse_input.dimension] != 0;
  }
}

} // namespace input
} // namespace kte
