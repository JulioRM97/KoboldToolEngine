#pragma once

#include <deque>
#include <memory>

#include "kte/state_machine/blackboard.h"

namespace kte
{

class GraphNode;

class Graph
{
 public:
  Graph();

  virtual ~Graph();

  void startWith(std::shared_ptr<GraphNode> node);

  virtual void update();

  Blackboard blackboard_;

 private:
  std::shared_ptr<GraphNode> current_node_;
};

}


