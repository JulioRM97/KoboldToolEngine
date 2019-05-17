#pragma once

#include "kte/game/component.h"

#include <memory>

namespace kte
{

class AudioSource;

class AudioSourceComponent : public Component
{
 public:
  explicit AudioSourceComponent(Node& parent);

  ~AudioSourceComponent() override;

  void update() override;

  std::shared_ptr<AudioSource> audio_source_;
};

} // end namespace kte

