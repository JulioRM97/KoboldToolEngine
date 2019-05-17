#pragma once

#include "kte/game/component.h"

#include <string>

namespace kte
{

class VrPoseTrackerComponent : public Component
{
 public:
  explicit VrPoseTrackerComponent(Node& parent);

  ~VrPoseTrackerComponent() override;

  void fixedUpdate() override;

 private:
  void editorSettings() override;
};

} // end namespace kte


