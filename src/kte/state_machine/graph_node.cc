#include "kte/state_machine/graph_node.h"

namespace kte
{

////////////////////////////////////////////////////////////////
GraphNode::GraphNode(std::function<void()> on_enter,
                     std::function<void()> update)
{
  on_enter_ = on_enter;
  update_ = update;
}

////////////////////////////////////////////////////////////////
GraphNode::~GraphNode() = default;

////////////////////////////////////////////////////////////////
void GraphNode::addTransition(std::shared_ptr<GraphNode> next,
                              std::function<bool(Blackboard&)> test_transition)
{
  transitions_.emplace_back(next, test_transition);
}

////////////////////////////////////////////////////////////////
std::shared_ptr<class GraphNode> GraphNode::executeTransitions(Blackboard& blackboard)
{
  for (const auto& transition : transitions_)
  {
    if (transition.test_transition_(blackboard))
    {
      return transition.next_;
    }
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////
void GraphNode::onEnter()
{
  if (on_enter_)
  {
    on_enter_();
  }
}

////////////////////////////////////////////////////////////////
void GraphNode::update()
{
  if (update_)
  {
    update_();
  }
}

} // namespace kte