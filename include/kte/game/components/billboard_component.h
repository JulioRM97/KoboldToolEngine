#pragma once

#include "kte/game/components/drawable_component.h"

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

namespace kte
{

// Forward declarations
namespace graphics
{
class DisplayList;
class Sprite;
}

class BillboardComponent : public DrawableComponent
{
 public:
  explicit BillboardComponent(Node& parent);

  ~BillboardComponent() override;

  void update() override;

  void draw(const kte::CameraParameters& camera_parameters,
            const LightComponent* light,
            kte::graphics::DisplayList& list,
            kte::graphics::RenderMode mode) const override;

  std::shared_ptr<kte::graphics::Sprite> sprite_;
};

} // end namespace kte

