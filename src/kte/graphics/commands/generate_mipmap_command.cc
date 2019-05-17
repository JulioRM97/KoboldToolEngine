#include "kte/graphics/commands/generate_mipmap_command.h"

#include "kte/graphics/texture.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
GenerateMipmapCommand::GenerateMipmapCommand(const std::shared_ptr<Texture> texture)
    : texture_(texture)
{
}

////////////////////////////////////////////////////////////////
GenerateMipmapCommand::~GenerateMipmapCommand() = default;

////////////////////////////////////////////////////////////////
void GenerateMipmapCommand::exec() const
{
  texture_->generateMipmap();
}

} // end namespace graphics
} // end namespace kte

