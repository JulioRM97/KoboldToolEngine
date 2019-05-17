#include "kte/game/components/orbit_movement_component.h"

#include <cmath>

#include <glm/ext.hpp>

#include "kte/game/game_manager.h"
#include "kte/game/node.h"
#include "kte/game/components/transform_component.h"
#include "kte/graphics/render_manager.h"
#include "kte/game/components/orbit_movement_component.h"
#include "kte/timer.h"

namespace kte
{

////////////////////////////////////////////////////////////////
OrbitMovementComponent::OrbitMovementComponent(Node& parent)
    : Component(parent)
{
}

////////////////////////////////////////////////////////////////
OrbitMovementComponent::OrbitMovementComponent(Node& parent,
                                               float axis_a,
                                               float axis_b,
                                               float orbit_speed,
                                               float radial_speed)
    : Component(parent),
      orbit_speed_(orbit_speed),
      radial_speed_(radial_speed)
{
  axis_a_ *= axis_a;
  axis_b_ *= axis_b;
}

////////////////////////////////////////////////////////////////
OrbitMovementComponent::OrbitMovementComponent(Node& parent,
                                               glm::vec3 axis_a,
                                               glm::vec3 axis_b,
                                               float orbit_speed,
                                               float radial_speed)
    : Component(parent),
      axis_a_(axis_a),
      axis_b_(axis_b),
      orbit_speed_(orbit_speed),
      radial_speed_(radial_speed)
{
}

////////////////////////////////////////////////////////////////
OrbitMovementComponent::~OrbitMovementComponent() = default;

////////////////////////////////////////////////////////////////
void OrbitMovementComponent::update()
{
  auto a = axis_a_ * cosf(Time::elapsedTime * orbit_speed_);
  auto b = axis_b_ * sinf(Time::elapsedTime * orbit_speed_);

  if (orbit_speed_ != 0.0f)
  {
    owner_node_.transform()->set_local_translation(a + b);
  }

  if (radial_speed_ != 0.0f)
  {
    auto up = glm::cross(glm::normalize(axis_a_), glm::normalize(axis_b_));
    auto rotation = glm::angleAxis(Time::elapsedTime * radial_speed_, up);

    owner_node_.transform()->set_local_rotation(rotation);
  }
}

} // end namespace kte

