#pragma once

#include "kte/game/components/drawable_component.h"

#include <memory>
#include <string>
#include <kte/graphics/drawables/skybox.h>

namespace kte
{

class SkyboxComponent : public DrawableComponent
{
 public:
  explicit SkyboxComponent(Node& parent);

  ~SkyboxComponent() override;

  void draw(const kte::CameraParameters& camera_parameters,
            const LightComponent* light,
            kte::graphics::DisplayList& list,
            kte::graphics::RenderMode mode) const override;

  std::shared_ptr<kte::graphics::Skybox> skybox_;
};

} // end namespace kte

