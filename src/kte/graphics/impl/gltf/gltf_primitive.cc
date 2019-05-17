#include "kte/graphics/impl/gltf/gltf_primitive.h"
#include "kte/graphics/gl_internals.h"
#include "gltf_internals.h"

#include <iostream>

#include "kte/game/game_manager.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
GLTFPrimitive::GLTFPrimitive() = default;

////////////////////////////////////////////////////////////////
GLTFPrimitive::~GLTFPrimitive() = default;

////////////////////////////////////////////////////////////////
void GLTFPrimitive::loadData(gltf_shared_ptr<tinygltf::Primitive> primitive)
{
  auto model_data = primitive.model_data();

  if (primitive->indices < 0) { return; }

  if (primitive->material >= 0)
  {
    material_ = model_data->materials_[primitive->material];
  }
  else
  {
    material_ = std::make_shared<kte::graphics::GLTFMaterial>();
  }
  data_->material_ = material_;

  for (auto& attribute : primitive->attributes)
  {
    auto& accessor = model_data->gltf_model_.accessors[attribute.second];
    data_->accessors_[attribute.second] = make_gltf_shared_ptr(&accessor, model_data);
    data_->buffers_[accessor.bufferView] = model_data->buffers_[accessor.bufferView];
  }

  auto& index_accessor = model_data->gltf_model_.accessors[primitive->indices];
  data_->accessors_[primitive->indices] = make_gltf_shared_ptr(&index_accessor, model_data);
  data_->buffers_[index_accessor.bufferView] = model_data->buffers_[index_accessor.bufferView];

  data_->primitive_ = primitive;

  data_->cpu_version_++;
}

} // end namespace graphics
} // end namespace kte

