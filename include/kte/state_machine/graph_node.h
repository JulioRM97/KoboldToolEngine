#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <string>

#include "kte/state_machine/transition.h"

namespace kte
{

// Forward declarations
class GraphNode;
class Blackboard;

class GraphNode
{
 public:
  explicit GraphNode(std::function<void()> on_enter,
                     std::function<void()> update);

  virtual ~GraphNode();

  void addTransition(std::shared_ptr<GraphNode> next,
                     std::function<bool(Blackboard&)> test_transition);

  virtual std::shared_ptr<GraphNode> executeTransitions(Blackboard& blackboard);

  virtual void onEnter();

  virtual void update();

 private:
  std::deque<Transition> transitions_;

  std::function<void()> on_enter_;

  std::function<void()> update_;
};

} // namespace kte

