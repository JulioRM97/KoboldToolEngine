#pragma once

#include "kte/system/memory_manager.hpp"

namespace kte
{

class Node;

class NodeManager : public MemoryManager<Node>
{
};

} //namespace kte

