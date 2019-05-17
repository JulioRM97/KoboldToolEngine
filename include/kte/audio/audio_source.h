#pragma once

#include <glm/vec3.hpp>

namespace kte
{

/// This can be any source of audio.
class AudioSource
{
 public:
  /// Play this audio source.
  virtual void play() = 0;

  /// Stop this audio source.
  virtual void stop() = 0;

  /// \param position The new position of this audio source.
  virtual void set_position(glm::vec3 position) = 0;

  /// \param velocity The new velocity of this audio source.
  virtual void set_velocity(glm::vec3 velocity) = 0;

  /// \param loop Defines if this audio should loop.
  virtual void set_looping(bool loop) = 0;

  /// \returns true if this audio source is looping.
  virtual bool is_looping() const = 0;

  /// \param gain The new gain of this audio source.
  virtual void set_gain(float gain) = 0;

  /// \returns the current gain of this source.
  virtual float gain() const = 0;

  /// \param pitch The new pitch of this audio source.
  virtual void set_pitch(float pitch) = 0;

  /// \returns the current pitch of this source.
  virtual float pitch() const = 0;

  /// \returns true if this audio source is playing.
  virtual bool is_playing() const = 0;

  virtual void* data() const = 0;
};

}

