#include "kte/state_machine/graph_manager.h"

#include "kte/state_machine/graph.h"

namespace kte
{

////////////////////////////////////////////////////////////////
std::shared_ptr<kte::Graph> GraphManager::createGraph()
{
  auto graph = std::make_shared<kte::Graph>();
  graphs_.insert(graph);
  return graph;
}

////////////////////////////////////////////////////////////////
void GraphManager::update()
{
  for (const auto& graph : graphs_)
  {
    graph->update();
  }
}

} // namespace kte

