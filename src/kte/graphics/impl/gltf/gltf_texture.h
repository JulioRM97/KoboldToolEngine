#pragma once

#include "kte/graphics/texture.h"

#include <memory>
#include <glm/vec2.hpp>

#include "gltf.h"

namespace kte
{

// Forward declarations
class GLTFGameLoader;
struct ModelData;

namespace graphics
{

class GLTFTexture : public Texture
{
 public:
  GLTFTexture();

  explicit GLTFTexture(gltf_shared_ptr<tinygltf::Texture> texture);

  explicit GLTFTexture(std::string file_name);

  ~GLTFTexture() override;

  void load() override;

  void generateMipmap() override;

  void unload() override;

  unsigned int internal_id() const override;

  glm::uvec2 size() const override;

  struct GPUData;

  std::shared_ptr<GPUData> data_;
};

} // end namespace graphics
} // end namespace kte

