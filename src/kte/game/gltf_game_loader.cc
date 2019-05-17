#include "kte/game/gltf_game_loader.h"
#include "kte/graphics/impl/gltf/gltf_internals.h"

#include <memory>

#include "kte/game/game_manager.h"
#include "kte/game/node_manager.h"
#include "kte/game/scene.h"
#include "kte/game/node.h"
#include "kte/game/components/camera_component.h"
#include "kte/game/components/mesh_component.h"
#include "kte/game/components/light_component.h"
#include "kte/graphics/drawables/primitive.h"
#include "kte/graphics/impl/gltf/gltf_buffer.h"
#include "kte/graphics/impl/gltf/gltf_material.h"
#include "kte/graphics/impl/gltf/gltf_primitive.h"
#include "kte/graphics/impl/gltf/gltf_texture.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
LightType toLightType(std::string gltfLightType)
{
  if (gltfLightType.compare("ambient") == 0)
  {
    return LightType::k_Ambient;
  }
  else if (gltfLightType.compare("directional") == 0)
  {
    return LightType::k_Directional;
  }
  else if (gltfLightType.compare("point") == 0)
  {
    return LightType::k_Point;
  }
  else if (gltfLightType.compare("spot") == 0)
  {
    return LightType::k_Spot;
  }

  return LightType::k_None;
}

////////////////////////////////////////////////////////////////
GLTFGameLoader::GLTFGameLoader()
    : model_data_(new ModelData())
{
}

////////////////////////////////////////////////////////////////
void GLTFGameLoader::load(Game& game, std::string filename, Scene* scene)
{
  // Load model from file
  tinygltf::TinyGLTF loader;
  std::string err;

  bool ret = loader.LoadASCIIFromFile(&model_data_->gltf_model_, &err, filename);

  if (!err.empty())
  {
    fprintf(stderr, "Err: %s\n", err.c_str());
  }

  if (!ret)
  {
    fprintf(stderr, "Failed to parse glTF\n");
    return;
  }

  // Textures from gltf file
  model_data_->textures_.reserve(model_data_->gltf_model_.textures.size());
  for (auto& texture : model_data_->gltf_model_.textures)
  {
    model_data_->textures_.push_back(std::make_shared<GLTFTexture>(
        make_gltf_shared_ptr(&texture, model_data_)));
  }

  // Load materials from gltf file
  model_data_->materials_.reserve(model_data_->gltf_model_.materials.size());
  for (auto& material : model_data_->gltf_model_.materials)
  {
    model_data_->materials_.push_back(std::make_shared<GLTFMaterial>(
        make_gltf_shared_ptr(&material, model_data_)));
  }

  // Load buffers from gltf file
  model_data_->buffers_.reserve(model_data_->gltf_model_.bufferViews.size());
  for (auto& bufferView : model_data_->gltf_model_.bufferViews)
  {
    model_data_->buffers_.push_back(std::make_shared<GLTFBuffer>(
        make_gltf_shared_ptr(&bufferView, model_data_)));
  }

  // Load scenes from gltf file
  if (scene)
  {
    int scene_idx = 0;
    for (auto& gltf_scene : model_data_->gltf_model_.scenes)
    {
      loadScene(*scene, scene_idx++);
    }
  }
  else
  {
    int scene_idx = 0;
    game.scenes_.resize(model_data_->gltf_model_.scenes.size());
    for (auto& scene : game.scenes_)
    {
      scene = std::make_shared<Scene>();
      scene->game_ = &game;
      loadScene(*scene, scene_idx++);
    }
  }

  game.current_scene_id_ = 0;
}

////////////////////////////////////////////////////////////////
void GLTFGameLoader::loadScene(Scene& scene, int scene_idx)
{
  auto& gltf_scene = model_data_->gltf_model_.scenes[scene_idx];

  // Add root nodes
  for (int node_id : gltf_scene.nodes)
  {
    loadNode(scene.addChild(), node_id);
  }
}

////////////////////////////////////////////////////////////////
void GLTFGameLoader::loadNode(Node& node, int node_id)
{
  auto& gltf_node = model_data_->gltf_model_.nodes[node_id];

  // Load transform data
  auto& matrix = gltf_node.matrix;
  if (matrix.size() == 16)
  {
    // Use matrix if available
    node.transform()->set_local_transform(glm::make_mat4x4(matrix.data()));
  }
  else
  {
    // Apply translation if available
    auto& translation = gltf_node.translation;
    if (translation.size() == 3)
    {
      node.transform()->set_local_translation(glm::make_vec3(translation.data()));
    }

    // Apply rotation if available
    auto& rotation = gltf_node.rotation;
    if (rotation.size() == 4)
    {
      node.transform()->set_local_rotation(glm::make_quat(rotation.data()));
    }

    // Apply scale if available
    auto& scale = gltf_node.scale;
    if (scale.size() == 3)
    {
      node.transform()->set_local_scale(glm::make_vec3(scale.data()));
    }
  }

  // Create mesh component
  auto mesh_id = gltf_node.mesh;
  if (mesh_id >= 0 && ((size_t) mesh_id) < model_data_->gltf_model_.meshes.size())
  {
    auto mesh = node.createComponent<MeshComponent>();
    const auto gltf_mesh = &model_data_->gltf_model_.meshes[mesh_id];

    mesh->primitives_.reserve(gltf_mesh->primitives.size());
    for (auto& primitive : gltf_mesh->primitives)
    {
      auto gltfPrimitive = new GLTFPrimitive();
      gltfPrimitive->loadData(make_gltf_shared_ptr(&primitive, model_data_));
      mesh->primitives_.emplace_back(gltfPrimitive);
    }
  }

  // Add camera component
  auto camera_idx = gltf_node.camera;
  if (camera_idx >= 0 && (size_t) camera_idx < model_data_->gltf_model_.cameras.size())
  {
    auto* camera_data = &model_data_->gltf_model_.cameras[camera_idx];
    auto* camera = node.createComponent<CameraComponent>();

    if (camera_data->type == "perspective")
    {
      camera->set_perspective(camera_data->perspective.yfov,
                              camera_data->perspective.aspectRatio,
                              camera_data->perspective.znear,
                              camera_data->perspective.zfar);
    }
    else if (camera_data->type == "orthographic")
    {
      camera->set_orthographic(camera_data->orthographic.xmag,
                               camera_data->orthographic.ymag,
                               camera_data->orthographic.znear,
                               camera_data->orthographic.zfar);
    }

    GameManager::Instance().current_camera_ = camera;
  }

  // Add light component
  auto light_data = gltf_node.extLightsValues;

  if (!light_data.empty())
  {
    // FIXME read the right light (not always light 0)
    // TODO there can be lights assigned only to the scene (ambient lights)
    tinygltf::Light& gltf_light = model_data_->gltf_model_.lights[0];
    auto light = node.createComponent<LightComponent>();
    light->set_type(toLightType(gltf_light.type));
    light->set_color(glm::make_vec3(gltf_light.color.data()));
  }

  // Add children
  for (int child_id : gltf_node.children)
  {
    loadNode(node.addChild(), child_id);
  }
}

} // end namespace kte


