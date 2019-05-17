#include "kte/graphics/texture.h"

#include "kte/graphics/impl/gltf/gltf_texture.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
std::shared_ptr<Texture> Texture::createFromFile(std::string file_name)
{
  return std::make_shared<GLTFTexture>(file_name);
}

} // end namespace graphics
} // end namespace kte
