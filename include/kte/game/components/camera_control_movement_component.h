#pragma once

#include "kte/game/component.h"

#include <string>

namespace kte
{

class CameraControlMovementComponent : public Component
{
 public:
  explicit CameraControlMovementComponent(Node& parent);

  ~CameraControlMovementComponent() override;

  void update() override;

  void registerDefaultInput();

  float cam_speed_ = 5.0f;

  float rotate_speed_ = 0.5f;

 private:
  void editorSettings() override;

  float yaw_ = 0.0f;

  float pitch_ = 0.0f;
};

} // end namespace kte


