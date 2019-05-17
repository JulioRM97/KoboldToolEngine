#pragma once

#include "kte/game/updatable.h"

#include <imgui.h>

#include <functional>
#include <deque>

#include "kte/game/component.h"
#include "kte/game/node.h"

namespace kte
{

namespace graphics
{
class Window;
} // namespace graphics

class EditorManager : public DefaultUpdatable
{
 public:
  EditorManager();

  virtual ~EditorManager();

  void update() override;

  void draw();

 private:
  void node(std::deque<Node*> nodes);

  void components(std::deque<Component*> comp);

  bool opened_ = true;
};

} // namespace kte

