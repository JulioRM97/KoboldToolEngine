#include "kte/graphics/buffer.h"
#include "kte/graphics/gl_internals.h"

#include <iostream>

#include "kte/game/game_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void Buffer::GPUData::load()
{
  if (cpu_version_ <= gpu_version_)
  {
    // Buffer is up to date... do not update
    return;
  }

  uint32_t next_version = cpu_version_;

  if (buffer_id_ == 0)
  {
    glGenBuffers(1, &buffer_id_);
  }

  glBindBuffer(target_, buffer_id_);
  glBufferData(target_, data_length_, buffer_data_, GL_STATIC_DRAW);
  glBindBuffer(target_, 0);
  checkErrors("glBufferData");

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Buffer::GPUData::unload()
{
  if (buffer_id_ == 0)
  {
    // Buffer is unloaded, no need to do anything else
    return;
  }

  glDeleteBuffers(1, &buffer_id_);
  checkErrors("glDeleteBuffers");
  buffer_id_ = 0;
}

////////////////////////////////////////////////////////////////
Buffer::Buffer()
    : data_(new GPUData())
{
}

////////////////////////////////////////////////////////////////
Buffer::~Buffer()
{
  unload();
};

////////////////////////////////////////////////////////////////
void Buffer::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void Buffer::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&Buffer::GPUData::unload, data_), true);
}

} // end namespace graphics
} // end namespace kte

