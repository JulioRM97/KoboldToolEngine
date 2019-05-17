#pragma once

#include "kte/game/component.h"

#include <memory>

#include "kte/types.h"

namespace kte
{

// Forward declarations
class CameraComponent;
class LightComponent;

namespace graphics
{
class DisplayList;
}

class DrawableComponent : public Component
{
 public:
  explicit DrawableComponent(Node& parent);

  ~DrawableComponent() override = default;

  virtual void draw(const kte::CameraParameters& camera_parameters,
                    const LightComponent* light,
                    kte::graphics::DisplayList& list,
                    kte::graphics::RenderMode mode) const = 0;
};

} // end namespace kte

