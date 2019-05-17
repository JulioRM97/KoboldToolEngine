#pragma once

#include "kte/graphics/drawables/primitive.h"

#include <memory>

#include "gltf.h"

namespace kte
{

class MeshComponent;

struct ModelData;

namespace graphics
{

class GLTFPrimitive : public Primitive
{
 public:
  GLTFPrimitive();

  ~GLTFPrimitive() override;

  void loadData(gltf_shared_ptr<tinygltf::Primitive> primitive);
};

} // end namespace graphics
} // end namespace kte

