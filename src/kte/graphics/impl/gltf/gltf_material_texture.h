#pragma once

#include <memory>
#include <string>

#include <glm/vec4.hpp>

namespace kte
{
namespace graphics
{

class Texture;
struct ProgramInfo;

class GLTFMaterialTexture
{
 public:
  GLTFMaterialTexture();

  ~GLTFMaterialTexture();

  void setupProgram(uint8_t texture_unit, const ProgramInfo& info);

  std::string uniform_name_;

  std::shared_ptr<kte::graphics::Texture> texture_;

  /// A factor to multiply the texture by. This can be "scale" for normal
  /// textures or "strength" for occlusion textures for example.
  glm::vec4 factor_{1};

  /// Whether to use the first or second texture coordinate (base 0).
  int tex_coord_ = 0;

 private:
  struct Data;

  std::unique_ptr<Data> data_;
};

} // end namespace graphics
} // end namespace kte


