#pragma once

#include "kte/game/updatable.h"

#include <string>
#include <memory>
#include <unordered_map>

#include "kte/types.h"
#include "axis.h"
#include "button.h"

const uint8_t X = 0;
const uint8_t Y = 1;
const uint8_t Z = 2;

namespace kte
{

class Game;

namespace input
{

class InputManager : public DefaultUpdatable
{
 public:
  InputManager();

  ~InputManager();

  /// Reads all the input.
  void update() override;

  void addButtonKeyInput(std::string name, Key key);

  void addButtonMouseInput(std::string name, uint32_t button);

  void addAxisKeyInput(std::string name, Key key, float value, int dimension);

  void addAxisMouseInput(std::string name, int axis, float value, int dimension);

  std::unordered_map<std::string, Action> buttons_;

  std::unordered_map<std::string, Axis> axes_;

  bool any_button_up_ = false;

  bool any_axis_ = false;

 private:
  void readInput(Action& action);

  void readInput(Axis& axis);
};

} // namespace input
} // namespace kte

