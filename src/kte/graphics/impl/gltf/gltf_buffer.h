#pragma once

#include "kte/graphics/buffer.h"

#include <memory>

#include "gltf.h"

namespace kte
{

// Forward declarations
class GLTFGameLoader;
struct ModelData;

namespace graphics
{

class GLTFBuffer : public Buffer
{
 public:
  GLTFBuffer();

  explicit GLTFBuffer(gltf_shared_ptr<tinygltf::BufferView> buffer_view);

  ~GLTFBuffer() override;

  gltf_shared_ptr<tinygltf::BufferView> buffer_view_;

  gltf_shared_ptr<tinygltf::Buffer> buffer_;
};

} // end namespace graphics
} // end namespace kte

