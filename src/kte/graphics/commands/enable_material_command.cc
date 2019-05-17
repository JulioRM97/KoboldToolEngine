#include "kte/graphics/commands/enable_material_command.h"
#include "kte/common.h"

#include "kte/graphics/material.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
EnableMaterialCommand::EnableMaterialCommand(const std::shared_ptr<Material> material,
                                             RenderMode mode)
    : material_(material),
      mode_(mode)
{
}

////////////////////////////////////////////////////////////////
EnableMaterialCommand::~EnableMaterialCommand() = default;

////////////////////////////////////////////////////////////////
void EnableMaterialCommand::exec() const
{
  material_->setupProgram(mode_);
}

} // end namespace graphics
} // end namespace kte

