#pragma once

#include "audio_source.h"

#include <memory>

namespace kte
{

class AudioBuffer;

/// An audio source that gets its data from an audio buffer.
class BufferAudioSource : public AudioSource
{
 public:
  BufferAudioSource();

  ~BufferAudioSource();

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

  /// \param buffer The buffer from where to play this audio source.
  void set_buffer(const AudioBuffer& buffer);

  struct Data;

 private:
  std::unique_ptr<Data> data_;

};

}

