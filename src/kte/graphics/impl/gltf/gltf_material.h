#pragma once

#include "kte/graphics/material.h"

#include <glm/vec4.hpp>

#include "gltf.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class GLTFMaterialTexture;

class GLTFMaterial : public Material
{
 public:
  GLTFMaterial();

  explicit GLTFMaterial(gltf_shared_ptr<tinygltf::Material> material);

  ~GLTFMaterial() override;

  void setupProgram(RenderMode mode) override;

 private:
  struct Data;

  std::unique_ptr<Data> data_;

  std::shared_ptr<GLTFMaterialTexture> getTexture(std::string name);

  glm::vec4 getFactor(std::string factor_name);
};

} // end namespace graphics
} // end namespace kte

