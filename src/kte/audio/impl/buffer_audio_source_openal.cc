#ifdef KTE_AUDIO_LIB_OPENAL

#include "kte/audio/buffer_audio_source.h"

#include <AL/al.h>

#include "kte/audio/audio_buffer.h"

namespace kte
{

////////////////////////////////////////////////////////////////
struct BufferAudioSource::Data
{
  ALuint source_ = 0;

  void cleanup()
  {
    if (source_)
    {
      alDeleteSources(1, &source_);
      source_ = 0;
    }
  }
};

////////////////////////////////////////////////////////////////
BufferAudioSource::BufferAudioSource()
  : data_(new Data())
{
  alGenSources(1, &data_->source_);
}

////////////////////////////////////////////////////////////////
BufferAudioSource::~BufferAudioSource()
{
  data_->cleanup();
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::play()
{
  alSourcePlay(data_->source_);
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::stop()
{
  alSourceStop(data_->source_);
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_position(glm::vec3 position)
{
  alSource3f(data_->source_, AL_POSITION, position.x, position.y, position.z);
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_velocity(glm::vec3 velocity)
{
  alSource3f(data_->source_, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_looping(bool loop)
{
  alSourcei(data_->source_, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

////////////////////////////////////////////////////////////////
bool BufferAudioSource::is_looping() const
{
  ALint loop = AL_FALSE;
  alGetSourcei(data_->source_, AL_LOOPING, &loop);
  return static_cast<bool>(loop);
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_gain(float gain)
{
  alSourcef(data_->source_, AL_GAIN, gain);
}

////////////////////////////////////////////////////////////////
float BufferAudioSource::gain() const
{
  ALfloat gain = 0.0f;
  alGetSourcef(data_->source_, AL_GAIN, &gain);
  return gain;
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_pitch(float pitch)
{
  alSourcef(data_->source_, AL_PITCH, pitch);
}

////////////////////////////////////////////////////////////////
float BufferAudioSource::pitch() const
{
  ALfloat pitch = 0.0f;
  alGetSourcef(data_->source_, AL_PITCH, &pitch);
  return pitch;
}

////////////////////////////////////////////////////////////////
bool BufferAudioSource::is_playing() const
{
  ALenum state;
  alGetSourcei(data_->source_, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

////////////////////////////////////////////////////////////////
void* BufferAudioSource::data() const
{
  return data_.get();
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_buffer(const AudioBuffer& buffer)
{
  alSourcei(data_->source_, AL_BUFFER, *static_cast<ALuint*>(buffer.data()));
}

}

#endif

