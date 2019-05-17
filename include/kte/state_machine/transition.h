#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <string>

namespace kte
{

class GraphNode;

class Blackboard;

struct Transition
{
  Transition();

  Transition(const Transition& other);

  Transition(Transition&& other);

  Transition(std::shared_ptr<GraphNode> next,
             std::function<bool(Blackboard&)> on_transition);

  ~Transition() = default;

  std::function<bool(Blackboard&)> test_transition_;

  std::shared_ptr<GraphNode> next_;
};

} // namespace kte

