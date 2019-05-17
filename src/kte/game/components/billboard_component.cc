#include "kte/game/components/billboard_component.h"

#include "kte/graphics/commands/draw_drawable_command.h"
#include "kte/graphics/display/display_list_buffer.h"
#include "kte/graphics/drawables/sprite.h"
#include "kte/game/node.h"
#include "kte/game/game_manager.h"
#include "kte/game/components/camera_component.h"
#include "kte/game/components/transform_component.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
BillboardComponent::BillboardComponent(Node& parent)
    : DrawableComponent(parent),
      sprite_(std::make_shared<Sprite>())
{
}

////////////////////////////////////////////////////////////////
BillboardComponent::~BillboardComponent() = default;

////////////////////////////////////////////////////////////////
void BillboardComponent::update()
{
  auto& camera = kte::GameManager::Instance().current_camera_->transform();

  auto billboard_pos = owner_node_.transform()->world_translation();
  auto camera_pos = camera.world_translation();

  auto dp = billboard_pos - camera_pos;
  auto orientation = glm::mat4_cast(glm::quat(glm::vec3(0, atan2f(dp.x, -dp.z), 0)));

  owner_node_.transform()->set_local_rotation(orientation);
}

////////////////////////////////////////////////////////////////
void BillboardComponent::draw(const kte::CameraParameters& camera_parameters,
                              const LightComponent* light,
                              kte::graphics::DisplayList& list,
                              kte::graphics::RenderMode mode) const
{
  auto world_transform = owner_node_.transform()->world_transform();


  list.add<DrawDrawableCommand>(sprite_, world_transform, camera_parameters, light, mode);
}

} // end namespace kte

