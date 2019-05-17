#pragma once

#include "kte/game/component.h"

namespace kte
{

class AudioListenerComponent : public Component
{
 public:
  explicit AudioListenerComponent(Node& parent);

  ~AudioListenerComponent() override;

  void update() override;

  bool active_ = true;
};

} // end namespace kte

