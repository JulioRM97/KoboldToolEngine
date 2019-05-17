#pragma once

#include <cstdint>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "kte/types.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class TextureAttachment;

enum class ZPrePassMode
{
  k_Normal,
  k_OnlyZ,
  k_ZEqual
};

class Framebuffer
{
 public:
  static const std::shared_ptr<Framebuffer>& ScreenFramebuffer();

  struct GPUData;

  Framebuffer();

  ~Framebuffer();

  std::shared_ptr<TextureAttachment> addColorTextureAttachment(bool generate_mipmaps = false);

  void bindColorTextureAttachment(std::shared_ptr<TextureAttachment> color_texture);

  void unbindColorTextureAttachment(uint8_t color_idx = 0);

  std::shared_ptr<TextureAttachment> addDataTextureAttachment();

  void bindDataTextureAttachment(std::shared_ptr<TextureAttachment> data_texture);

  std::shared_ptr<TextureAttachment> addDepthTextureAttachment();

  void bindDepthTextureAttachment(std::shared_ptr<TextureAttachment> depth_texture);

  void unbindDepthTextureAttachment();

  std::shared_ptr<TextureAttachment> addStencilTextureAttachment();

  void bindStencilTextureAttachment(std::shared_ptr<TextureAttachment> stencil_texture);

  void toggleBlend(bool enable) const;

  void enableViewport(rect2i viewport) const;

  void clear(glm::vec4 color);

  void use(ZPrePassMode mode = ZPrePassMode::k_Normal) const;

  size_t num_color_textures() const;

  std::shared_ptr<TextureAttachment> color_texture(uint8_t color_idx = 0) const;

  std::shared_ptr<TextureAttachment> depth_texture() const;

  std::shared_ptr<TextureAttachment> stencil_texture() const;

  glm::ivec2 size() const;

  void set_size(glm::ivec2 size);

  int internal_id() const;

  std::shared_ptr<GPUData> data_;

 private:
  /// Used to construct the default or screen framebuffer
  explicit Framebuffer(int);

  glm::ivec2 default_size_{128, 128};

};

} // end namespace graphics
} // end namespace kte

