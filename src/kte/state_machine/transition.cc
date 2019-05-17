#include "kte/state_machine/transition.h"

namespace kte
{

////////////////////////////////////////////////////////////////
Transition::Transition() = default;

////////////////////////////////////////////////////////////////
Transition::Transition(const Transition& other) = default;

////////////////////////////////////////////////////////////////
Transition::Transition(Transition&& other) = default;

////////////////////////////////////////////////////////////////
Transition::Transition(std::shared_ptr<GraphNode> next,
                       std::function<bool(Blackboard&)> on_transition)
{
  test_transition_ = on_transition;
  next_ = next;
}

} // namespace kte

