#pragma once

#include "kte/game/component.h"

#include <string>
#include <glm/vec3.hpp>

namespace kte
{

class OrbitMovementComponent : public Component
{
 public:
  explicit OrbitMovementComponent(Node& parent);

  OrbitMovementComponent(Node& parent,
                         float axis_a,
                         float axis_b,
                         float orbit_speed = 1.0f,
                         float radial_speed = 0.0f);

  OrbitMovementComponent(Node& parent,
                         glm::vec3 axis_a,
                         glm::vec3 axis_b,
                         float orbit_speed = 1.0f,
                         float radial_speed = 0.0f);

  ~OrbitMovementComponent() override;

  void update() override;

  glm::vec3 axis_a_{1.0f, 0.0f, 0.0f};

  glm::vec3 axis_b_{0.0f, 0.0f, 1.0f};

  float orbit_speed_ = 1.0f;

  float radial_speed_ = 0.0f;

 private:
  void editorSettings() override;
};

} // end namespace kte


