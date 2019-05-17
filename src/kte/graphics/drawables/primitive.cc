#include "kte/graphics/drawables/primitive.h"
#include "kte/graphics/gl_internals.h"
#include "kte/graphics/impl/gltf/gltf_internals.h"

#include <iostream>

#include "kte/game/game_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void Primitive::GPUData::load()
{
  if (cpu_version_ <= gpu_version_)
  {
    // Buffer is up to date... do not update
    return;
  }

  uint32_t next_version = cpu_version_;

  if (primitive_->indices < 0) { return; }

  // Generate Vertex Array for this primitive
  if (!vao_id_)
  {
    glGenVertexArrays(1, &vao_id_);
  }

  glBindVertexArray(vao_id_);

  // Load vertex attributes
  for (auto& attribute : primitive_->attributes)
  {
    int attribute_idx = material_->program_->data_->attributes[attribute.first];
    if (attribute_idx >= 0)
    {
      // FIXME extract this to somewhere gltf only
      const auto& accessor = *accessors_[attribute.second];
      const auto buffer = buffers_[accessor.bufferView];

      buffer->load();

      glBindBuffer(GL_ARRAY_BUFFER, buffer->data_->buffer_id_);
      checkErrors("glBindBuffer");

      glVertexAttribPointer(attribute_idx,
                            size(accessor),
                            accessor.componentType,
                            accessor.normalized ? GL_TRUE : GL_FALSE,
                            buffer->data_->byte_stride_,
                            reinterpret_cast<void*>(accessor.byteOffset));
      checkErrors("glVertexAttribPointer");

      glEnableVertexAttribArray(attribute_idx);
      checkErrors("glEnableVertexAttribArray");
    }
  }

  const auto& index_accessor = *accessors_[primitive_->indices];
  const auto index_buffer = buffers_[index_accessor.bufferView];

  index_buffer->load();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer->data_->buffer_id_);
  checkErrors("glBindBuffer");

  glBindVertexArray(0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Primitive::GPUData::unload()
{
  if (vao_id_)
  {
    glDeleteVertexArrays(1, &vao_id_);
  }
}

////////////////////////////////////////////////////////////////
Primitive::Primitive()
    : data_(new GPUData())
{
}

////////////////////////////////////////////////////////////////
Primitive::~Primitive()
{
  unload();
}

////////////////////////////////////////////////////////////////
void Primitive::load()
{
  data_->load();

  for (auto& buffer : data_->buffers_)
  {
    buffer.second->load();
  }
}

////////////////////////////////////////////////////////////////
void Primitive::draw() const
{
  const auto& gltf_primitive = *data_->primitive_;
  const auto& index_accessor = *data_->accessors_[gltf_primitive.indices];

  glBindVertexArray(data_->vao_id_);
  checkErrors("glBindVertexArray");

  glDrawElements(glMode(gltf_primitive),
                 index_accessor.count,
                 index_accessor.componentType,
                 reinterpret_cast<void*>(index_accessor.byteOffset));
  checkErrors("glDrawElements");

  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////
void Primitive::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&Primitive::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
Program& Primitive::program() const
{
  return *material_->program_;
}

} // end namespace graphics
} // end namespace kte

