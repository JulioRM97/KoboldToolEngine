#include "kte/graphics/framebuffer.h"
#include "gl_internals.h"

#include <cstdint>

#include "kte/game/game_manager.h"
#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/window.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
const std::shared_ptr<Framebuffer>& Framebuffer::ScreenFramebuffer()
{
  static const std::shared_ptr<Framebuffer> gScreenFramebuffer(new Framebuffer(0));
  return gScreenFramebuffer;
}

////////////////////////////////////////////////////////////////
void Framebuffer::GPUData::load()
{
  uint32_t next_version = cpu_version_;

  if (next_version <= gpu_version_)
  {
    return;
  }

  if (framebuffer_id == 0)
  {
    glGenFramebuffers(1, &framebuffer_id);
    checkErrors("glGenFramebuffers");
    if (!framebuffer_id)
    {
      return;
    }
  }

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_id);
  checkErrors("glBindFramebuffer");

  clear_flags = GL_COLOR_BUFFER_BIT;
  attachments.clear();
  attachments.reserve(color_textures.size());

  // color textures
  uint8_t id = 0;
  for (auto& color_texture : color_textures)
  {
    color_texture->data_->color_idx = id;
    color_texture->load();
    color_texture->attach();
    attachments.push_back(GL_COLOR_ATTACHMENT0 + id);
    id++;
  }

  if (depth_texture)
  {
    depth_texture->load();
    depth_texture->attach();
    clear_flags |= GL_DEPTH_BUFFER_BIT;
  }

  if (stencil_texture)
  {
    stencil_texture->load();
    stencil_texture->attach();
    clear_flags |= GL_STENCIL_BUFFER_BIT;
  }

  checkFrameBufferErrors("Framebuffer::GPUData::load");
  checkErrors("Framebuffer::GPUData::load");

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Framebuffer::GPUData::unload()
{
  for (auto& color_texture : color_textures)
  {
    color_texture->unload();
  }

  if (depth_texture)
  {
    depth_texture->unload();
  }

  if (stencil_texture)
  {
    stencil_texture->unload();
  }
}

////////////////////////////////////////////////////////////////
Framebuffer::Framebuffer()
    : data_(std::make_shared<GPUData>())
{
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
Framebuffer::Framebuffer(int)
    : data_(std::make_shared<GPUData>())
{
  data_->framebuffer_id = 0;
  data_->clear_flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
  data_->gpu_version_ = data_->cpu_version_ = 0;
}

////////////////////////////////////////////////////////////////
Framebuffer::~Framebuffer()
{
  data_->unload();
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::addColorTextureAttachment(bool generate_mipmaps)
{
  auto color_texture = std::make_shared<TextureAttachment>();
  color_texture->set_type(TextureAttachment::Type::k_Color);
  color_texture->set_size(default_size_);
  color_texture->set_generate_mipmap(generate_mipmaps);
  data_->color_textures.push_back(color_texture);

  data_->cpu_version_++;

  return color_texture;
}

////////////////////////////////////////////////////////////////
void Framebuffer::bindColorTextureAttachment(std::shared_ptr<TextureAttachment> color_texture)
{
  color_texture->set_type(TextureAttachment::Type::k_Color);
  data_->color_textures.resize(color_texture->data_->color_idx + 1);
  data_->color_textures[color_texture->data_->color_idx] = color_texture;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
void Framebuffer::unbindColorTextureAttachment(uint8_t color_idx)
{
  use();
  auto& color_texture = data_->color_textures[color_idx];
  color_texture->detach();
  data_->color_textures.erase(data_->color_textures.begin() + color_idx);
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::addDataTextureAttachment()
{
  auto data_texture = std::make_shared<TextureAttachment>();
  data_texture->set_type(TextureAttachment::Type::k_Data);
  data_texture->set_size(default_size_);
  data_->color_textures.push_back(data_texture);

  data_->cpu_version_++;

  return data_texture;
}

////////////////////////////////////////////////////////////////
void Framebuffer::bindDataTextureAttachment(std::shared_ptr<TextureAttachment> data_texture)
{
  data_texture->set_type(TextureAttachment::Type::k_Data);
  data_->color_textures.push_back(data_texture);
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::addDepthTextureAttachment()
{
  data_->depth_texture = std::make_shared<TextureAttachment>();
  data_->depth_texture->set_type(TextureAttachment::Type::k_Depth);
  data_->depth_texture->set_size(default_size_);

  data_->cpu_version_++;

  return data_->depth_texture;
}

////////////////////////////////////////////////////////////////
void Framebuffer::bindDepthTextureAttachment(std::shared_ptr<TextureAttachment> depth_texture)
{
  depth_texture->set_type(TextureAttachment::Type::k_Depth);
  data_->depth_texture = depth_texture;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
void Framebuffer::unbindDepthTextureAttachment()
{
  use();
  data_->depth_texture->detach();
  data_->depth_texture = nullptr;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::addStencilTextureAttachment()
{
  data_->stencil_texture = std::make_shared<TextureAttachment>();
  data_->stencil_texture->set_type(TextureAttachment::Type::k_Stencil);
  data_->stencil_texture->set_size(default_size_);

  data_->cpu_version_++;

  return data_->stencil_texture;
}

////////////////////////////////////////////////////////////////
void Framebuffer::bindStencilTextureAttachment(std::shared_ptr<TextureAttachment> stencil_texture)
{
  stencil_texture->set_type(TextureAttachment::Type::k_Stencil);
  data_->stencil_texture = stencil_texture;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
void Framebuffer::toggleBlend(bool enable) const
{
  if (enable)
  {
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
  }
  else
  {
    glDisable(GL_BLEND);
  }
}

////////////////////////////////////////////////////////////////
void Framebuffer::enableViewport(rect2i viewport) const
{
  if (viewport.size.x | viewport.size.y)
  {
    glViewport(viewport.pos.x, viewport.pos.y, viewport.size.x, viewport.size.y);
  }
  else if (num_color_textures() > 0)
  {
    glm::ivec2 size = color_texture(0)->size();
    glViewport(0, 0, size.x, size.y);
  }
  else
  {
    // No size for viewport
  }
}

////////////////////////////////////////////////////////////////
void Framebuffer::clear(glm::vec4 color)
{
  data_->load();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, data_->framebuffer_id);
  glClearColor(color.r, color.g, color.b, color.a);
  glClearDepth(1.0);
  glClear(data_->clear_flags);
  glEnable(GL_DEPTH_TEST);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  checkErrors("Framebuffer::clear");
}

////////////////////////////////////////////////////////////////
void Framebuffer::use(ZPrePassMode mode) const
{
  data_->load();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, data_->framebuffer_id);
  checkErrors("glBindFramebuffer");

  switch (mode)
  {
    case ZPrePassMode::k_Normal:
    {
      glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthFunc(GL_LEQUAL);
      break;
    }

    case ZPrePassMode::k_OnlyZ:
    {
      glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glDepthFunc(GL_LEQUAL);
      break;
    }

    case ZPrePassMode::k_ZEqual:
    {
      glColorMaski(0, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glDepthFunc(GL_EQUAL);
      break;
    }
  }

  if (!data_->attachments.empty())
  {
    glDrawBuffers(static_cast<GLsizei>(data_->attachments.size()), data_->attachments.data());
    checkErrors("glDrawBuffers");
  }

  if (data_->srgb)
  {
    glEnable(GL_FRAMEBUFFER_SRGB);
  }
}

////////////////////////////////////////////////////////////////
size_t Framebuffer::num_color_textures() const
{
  return data_->color_textures.size();
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::color_texture(uint8_t color_idx) const
{
  return data_->color_textures[color_idx];
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::depth_texture() const
{
  return data_->depth_texture;
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TextureAttachment> Framebuffer::stencil_texture() const
{
  return data_->stencil_texture;
}

////////////////////////////////////////////////////////////////
glm::ivec2 Framebuffer::size() const
{
  return default_size_;
}

////////////////////////////////////////////////////////////////
void Framebuffer::set_size(glm::ivec2 size)
{
  default_size_ = size;

  for (auto& color_texture : data_->color_textures)
  {
    color_texture->set_size(size);
  }

  if (data_->depth_texture)
  {
    data_->depth_texture->set_size(size);
  }

  if (data_->stencil_texture)
  {
    data_->stencil_texture->set_size(size);
  }

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
int Framebuffer::internal_id() const
{
  return data_->framebuffer_id;
}

} // end namespace graphics
} // end namespace kte

