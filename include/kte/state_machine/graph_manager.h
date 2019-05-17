#pragma once

#include "kte/game/updatable.h"

#include <memory>
#include <set>

namespace kte
{

// Forward declarations
class Graph;

class GraphManager : public DefaultUpdatable
{
 public:
  std::shared_ptr<kte::Graph> createGraph();

  void update() override;

 private:
  std::set<std::shared_ptr<kte::Graph>> graphs_;
};

} // namespace kte

