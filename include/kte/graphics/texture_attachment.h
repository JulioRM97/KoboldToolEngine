#pragma once

#include "texture.h"

namespace kte
{
namespace graphics
{

class TextureAttachment : public Texture
{
 public:
  enum class Type : uint8_t
  {
    k_None = 0,
    k_Color,
    k_Data,
    k_Depth,
    k_Stencil
  };

  struct GPUData;

  TextureAttachment();

  TextureAttachment(unsigned int internal_id, glm::ivec2 size);

  ~TextureAttachment() override;

  void load() override;

  void generateMipmap() override;

  void unload() override;

  void attach();

  void detach();

  unsigned int internal_id() const override;

  TextureAttachment::Type type();

  void set_type(TextureAttachment::Type type);

  glm::uvec2 size() const override;

  void set_size(glm::ivec2 size);

  bool generate_mipmap() const;

  void set_generate_mipmap(bool generate_mipmaps);

  std::shared_ptr<GPUData> data_;
};

} // end namespace graphics
} // end namespace kte

