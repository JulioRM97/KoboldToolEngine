#pragma once

#include "texture.h"

namespace kte
{
namespace graphics
{

class DataTexture : public Texture
{
 public:
  struct GPUData;

  DataTexture();

  DataTexture(void* data, glm::ivec2 size);

  ~DataTexture() override;

  void load() override;

  void generateMipmap() override;

  void unload() override;

  unsigned int internal_id() const override;

  glm::uvec2 size() const override;

  void set_size(glm::ivec2 size);

  bool generate_mipmap() const;

  void set_generate_mipmap(bool generate_mipmaps);

  std::shared_ptr<GPUData> data_;
};

} // end namespace graphics
} // end namespace kte

