#include "kte/graphics/data_texture.h"
#include "kte/graphics/gl_internals.h"

#include "kte/game/game_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void DataTexture::GPUData::load()
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

  glBindTexture(GL_TEXTURE_2D, texture_id);
  checkErrors("glBindTexture");

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.x, size.y, 0,
               GL_RGB, GL_FLOAT, data ? data : nullptr);
  checkErrors("glTexImage2D");

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generate_mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, generate_mipmap ? GL_LINEAR : GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  checkErrors("glTexParameteri");

  glBindTexture(GL_TEXTURE_2D, 0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void DataTexture::GPUData::unload()
{
  if (texture_id)
  {
    glDeleteTextures(1, &texture_id);
    texture_id = 0;
  }
}

////////////////////////////////////////////////////////////////
DataTexture::DataTexture()
    : data_(std::make_shared<GPUData>())
{
}

////////////////////////////////////////////////////////////////
DataTexture::DataTexture(void* data, glm::ivec2 size)
    : data_(std::make_shared<GPUData>())
{
  data_->size = size;
  data_->data = data;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
DataTexture::~DataTexture()
{
  unload();
}

////////////////////////////////////////////////////////////////
void DataTexture::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void DataTexture::generateMipmap()
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
void DataTexture::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&DataTexture::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
unsigned int DataTexture::internal_id() const
{
  return data_->texture_id;
}

////////////////////////////////////////////////////////////////
glm::uvec2 DataTexture::size() const
{
  return data_->size;
}

////////////////////////////////////////////////////////////////
void DataTexture::set_size(glm::ivec2 size)
{
  data_->size = size;
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
bool DataTexture::generate_mipmap() const
{
  return data_->generate_mipmap;
}

////////////////////////////////////////////////////////////////
void DataTexture::set_generate_mipmap(bool generate_mipmaps)
{
  data_->generate_mipmap = generate_mipmaps;
  data_->cpu_version_++;
}

} // end namespace graphics
} // end namespace kte
