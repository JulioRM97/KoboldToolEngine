#include "kte/state_machine/graph.h"

#include "kte/state_machine/graph_node.h"

namespace kte
{

////////////////////////////////////////////////////////////////
Graph::Graph() = default;

////////////////////////////////////////////////////////////////
Graph::~Graph() = default;

////////////////////////////////////////////////////////////////
void Graph::update()
{
  current_node_->update();

  auto next_node = current_node_->executeTransitions(blackboard_);

  if (next_node)
  {
    current_node_ = next_node;
    current_node_->onEnter();
  }
}

////////////////////////////////////////////////////////////////
void Graph::startWith(std::shared_ptr<GraphNode> node)
{
  current_node_ = node;
  current_node_->onEnter();
}

} // namespace kte

