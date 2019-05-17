#pragma once

#include <memory>
#include <map>

#include <glad/glad.h>

#include "gltf_buffer.h"
#include "gltf_material.h"
#include "gltf_texture.h"
#include "gltf_shared_ptr.h"

namespace kte
{

/// Hidden model data passed around, should not be copied or moved.
struct ModelData
{
  ModelData() = default;
  ModelData(const ModelData& other) = delete;
  ModelData(ModelData&& other) noexcept = delete;
  ~ModelData() = default;

  tinygltf::Model gltf_model_;

  std::vector<std::shared_ptr<kte::graphics::GLTFMaterial>> materials_;

  std::vector<std::shared_ptr<kte::graphics::GLTFBuffer>> buffers_;

  std::vector<std::shared_ptr<kte::graphics::GLTFTexture>> textures_;
};

namespace graphics
{

/// \param the primitive to get the mode from
/// \returns the gl mode of a primitive
GLenum glMode(const tinygltf::Primitive& primitive);

/// \param accessor An accessor to get the size from
/// \returns the size of the provided accessor
int size(const tinygltf::Accessor& accessor);

/// Convert gltf to opengl param
GLenum gltfToGLenum(int gltf_value);

/// Convert the value and call glTexParameteri if the parameter is valid
void gltfTexParameteri(GLenum name, int gltf_value);

/// Convert the parameter to a vec4. If there's one component use that value
/// for all 4 axes, if there are 2 or 3, pad with 1, if there are 4, use all
/// of them.
glm::vec4 gltfParamToVec4(const tinygltf::Parameter& parameter);

} // end namespace graphics
} // end namespace kte

