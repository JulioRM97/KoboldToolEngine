#include "kte/graphics/impl/gltf/gltf_buffer.h"
#include "kte/graphics/gl_internals.h"
#include "gltf_internals.h"

#include <iostream>

#include "kte/game/game_manager.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
GLTFBuffer::GLTFBuffer() = default;

////////////////////////////////////////////////////////////////
GLTFBuffer::GLTFBuffer(gltf_shared_ptr<tinygltf::BufferView> buffer_view)
{
  auto model_data = buffer_view.model_data();
  buffer_view_ = buffer_view;

  buffer_.set(&model_data->gltf_model_.buffers[buffer_view_->buffer], model_data);

  if (!buffer_ || !buffer_view_ || buffer_view_->target == 0)
  {
    std::cerr << "WARN: buffer data is invalid" << std::endl;
    return;
  }

  data_->target_ = static_cast<GLenum>(buffer_view_->target);
  data_->buffer_data_ = &buffer_->data.at(0) + buffer_view_->byteOffset;
  data_->data_length_ = buffer_view_->byteLength;
  data_->byte_stride_ = static_cast<GLsizei>(buffer_view_->byteStride);

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
GLTFBuffer::~GLTFBuffer() = default;

} // end namespace graphics
} // end namespace kte

