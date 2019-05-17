#pragma once

#include "audio_source.h"

#include <memory>
#include <vector>

namespace kte
{

/// An audio source that has multiple layers.
class LayeredAudioSource : public AudioSource
{
 public:
  LayeredAudioSource();

  ~LayeredAudioSource();

  void play() override;

  void stop() override;

  void set_position(glm::vec3 position) override;

  void set_velocity(glm::vec3 velocity) override;

  void set_looping(bool loop) override;

  bool is_looping() const override;

  void set_gain(float gain) override;

  float gain() const override;

  void set_pitch(float pitch) override;

  float pitch() const override;

  bool is_playing() const override;

  void* data() const override;

  /// The layers of this audio source.
  std::vector<std::shared_ptr<AudioSource>> layers_;
};

}

