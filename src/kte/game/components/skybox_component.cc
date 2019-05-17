#include "kte/game/components/skybox_component.h"

#include "kte/graphics/commands/draw_drawable_command.h"
#include "kte/graphics/display/display_list_buffer.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
SkyboxComponent::SkyboxComponent(Node& parent)
    : DrawableComponent(parent),
      skybox_(std::make_shared<Skybox>())
{
}

////////////////////////////////////////////////////////////////
SkyboxComponent::~SkyboxComponent() = default;

////////////////////////////////////////////////////////////////
void SkyboxComponent::draw(const kte::CameraParameters& camera_parameters,
                           const LightComponent* light,
                           kte::graphics::DisplayList& list,
                           kte::graphics::RenderMode mode) const
{
  static glm::mat4 model_transform(1.0f);

  list.add<DrawDrawableCommand>(skybox_, model_transform, camera_parameters, light, mode);
}

} // end namespace kte

