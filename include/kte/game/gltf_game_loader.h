#pragma once

#include "game.h"

namespace kte
{

class Scene;
class Node;
struct ModelData;

class GLTFGameLoader
{
 public:
  GLTFGameLoader();

  void load(Game& game, std::string filename, Scene* scene = nullptr);

 private:
  void loadScene(Scene& scene, int scene_idx);

  void loadNode(Node& node, int node_id);

  std::shared_ptr<ModelData> model_data_;
};

} // end namespace kte

