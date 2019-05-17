#pragma once

#include "kte/game/components/drawable_component.h"

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

namespace kte
{

namespace graphics
{
class Primitive;
}

class MeshComponent : public DrawableComponent
{
 public:
  explicit MeshComponent(Node& parent);

  ~MeshComponent() override;

  void draw(const kte::CameraParameters& camera_parameters,
            const LightComponent* light,
            kte::graphics::DisplayList& list,
            kte::graphics::RenderMode mode) const override;

  std::vector<std::shared_ptr<kte::graphics::Primitive>> primitives_;
};

} // end namespace kte

