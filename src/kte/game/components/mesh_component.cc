#include "kte/game/components/mesh_component.h"

#include "kte/graphics/drawables/primitive.h"
#include "kte/graphics/commands/enable_material_command.h"
#include "kte/graphics/commands/draw_drawable_command.h"
#include "kte/graphics/display/display_list_buffer.h"
#include "kte/game/game_manager.h"
#include "kte/game/game.h"
#include "kte/game/node.h"
#include "kte/game/components/camera_component.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
MeshComponent::MeshComponent(Node& parent)
    : DrawableComponent(parent)
{
}

////////////////////////////////////////////////////////////////
MeshComponent::~MeshComponent() = default;

////////////////////////////////////////////////////////////////
void MeshComponent::draw(const kte::CameraParameters& camera_parameters,
                         const LightComponent* light,
                         kte::graphics::DisplayList& list,
                         kte::graphics::RenderMode mode) const
{
  const auto world_transform = owner_node_.transform()->world_transform();

  for (const auto& primitive : primitives_)
  {
    list.add<EnableMaterialCommand>(primitive->material_, mode);
    list.add<DrawDrawableCommand>(primitive, world_transform, camera_parameters, light, mode);
  }
}

} // end namespace kte

