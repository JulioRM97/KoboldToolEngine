#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/gl_internals.h"

#include "kte/game/game_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void TextureAttachment::GPUData::load()
{
  uint32_t next_version = cpu_version_;

  if (next_version <= gpu_version_)
  {
    return;
  }

  if (!texture_id)
  {
    glGenTextures(1, &texture_id);
    checkErrors("glGenTextures");
  }
  else if (size.x == 0 || size.y == 0)
  {
    glm::ivec2 sizef;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sizef.x);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &sizef.y);
    size = sizef;
  }

  glBindTexture(GL_TEXTURE_2D, texture_id);
  checkErrors("glBindTexture");

  if (init_texture)
  {
    switch (type)
    {
      case Type::k_None:
      {
        break;
      }

      case Type::k_Color:
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0,
                     GL_RGBA, GL_FLOAT, nullptr);
        checkErrors("glTexImage2D");
        break;
      }

      case Type::k_Data:
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.x, size.y, 0,
                     GL_RGB, GL_FLOAT, nullptr);
        checkErrors("glTexImage2D");
        break;
      }

      case Type::k_Depth:
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, size.x, size.y,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        checkErrors("glTexImage2D");
        break;
      }

      case Type::k_Stencil:
      {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y,
                     0, GL_RGBA, GL_FLOAT, nullptr);
        checkErrors("glTexImage2D");
        break;
      }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generate_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, generate_mipmap ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    checkErrors("glTexParameteri");
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void TextureAttachment::GPUData::unload()
{
  if (texture_id)
  {
    glDeleteTextures(1, &texture_id);
    texture_id = 0;
  }
}

////////////////////////////////////////////////////////////////
TextureAttachment::TextureAttachment()
    : data_(std::make_shared<GPUData>())
{
}

////////////////////////////////////////////////////////////////
TextureAttachment::TextureAttachment(unsigned int internal_id, glm::ivec2 size)
    : data_(std::make_shared<GPUData>())
{
  data_->texture_id = internal_id;
  data_->size = size;
  data_->init_texture = false;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
TextureAttachment::~TextureAttachment()
{
  unload();
}

////////////////////////////////////////////////////////////////
void TextureAttachment::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void TextureAttachment::generateMipmap()
{
  if (data_->generate_mipmap)
  {
    glBindTexture(GL_TEXTURE_2D, internal_id());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    checkErrors("glGenerateMipmap");
  }
}

////////////////////////////////////////////////////////////////
void TextureAttachment::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&TextureAttachment::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
void TextureAttachment::attach()
{
  // color textures
  switch (data_->type)
  {
    case Type::k_None:
    {
      break;
    }

    case Type::k_Color:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + data_->color_idx,
                             GL_TEXTURE_2D, data_->texture_id, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Data:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + data_->color_idx,
                             GL_TEXTURE_2D, data_->texture_id, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Depth:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_2D, data_->texture_id, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Stencil:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_STENCIL_ATTACHMENT,
                             GL_TEXTURE_2D, data_->texture_id, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }
  }
}

////////////////////////////////////////////////////////////////
void TextureAttachment::detach()
{
  // color textures
  switch (data_->type)
  {
    case Type::k_None:
    {
      break;
    }

    case Type::k_Color:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + data_->color_idx,
                             GL_TEXTURE_2D, 0, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Data:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0 + data_->color_idx,
                             GL_TEXTURE_2D, 0, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Depth:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_DEPTH_ATTACHMENT,
                             GL_TEXTURE_2D, 0, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }

    case Type::k_Stencil:
    {
      glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                             GL_STENCIL_ATTACHMENT,
                             GL_TEXTURE_2D, 0, 0);
      checkErrors("glFramebufferTexture2D");
      break;
    }
  }
}

////////////////////////////////////////////////////////////////
unsigned int TextureAttachment::internal_id() const
{
  return data_->texture_id;
}

////////////////////////////////////////////////////////////////
TextureAttachment::Type TextureAttachment::type()
{
  return data_->type;
}

////////////////////////////////////////////////////////////////
void TextureAttachment::set_type(TextureAttachment::Type type)
{
  data_->type = type;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
glm::uvec2 TextureAttachment::size() const
{
  return data_->size;
}

////////////////////////////////////////////////////////////////
void TextureAttachment::set_size(glm::ivec2 size)
{
  data_->size = size;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
bool TextureAttachment::generate_mipmap() const
{
  return data_->generate_mipmap;
}

////////////////////////////////////////////////////////////////
void TextureAttachment::set_generate_mipmap(bool generate_mipmaps)
{
  data_->generate_mipmap = generate_mipmaps;
  data_->cpu_version_++;
}

} // end namespace graphics
} // end namespace kte
