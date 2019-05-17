#ifdef KTE_AUDIO_LIB_FMOD

#include "kte/audio/buffer_audio_source.h"
#include "fmod_internals.h"

#include "kte/audio/audio_buffer.h"

namespace kte
{

////////////////////////////////////////////////////////////////
BufferAudioSource::BufferAudioSource()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
BufferAudioSource::~BufferAudioSource() = default;

////////////////////////////////////////////////////////////////
void BufferAudioSource::play()
{
  gSystem->playSound(data_->sound_, nullptr, false, &data_->channel_);
  data_->sound_->set3DMinMaxDistance(1.0f, 500.0f);
  
  data_->channel_->getIndex(&data_->channel_index_);

  const FMOD_VECTOR
      pos = {data_->position_.x, data_->position_.y, data_->position_.z};
  const FMOD_VECTOR
      vel = {data_->velocity_.x, data_->velocity_.y, data_->velocity_.z};
   
  data_->channel_->setMode(FMOD_3D);
  if (data_->channel_->set3DMinMaxDistance(1.0f, 100.0f) != FMOD_OK)
	  printf("Error\n");
  if (data_->channel_->set3DAttributes(&pos, &vel) != FMOD_OK)
    printf("Error channel\n");

}

////////////////////////////////////////////////////////////////
void BufferAudioSource::stop()
{
  if (data_->channel_)
  {
    data_->channel_->stop();
  }
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_position(glm::vec3 position)
{
  data_->position_ = position;
  
  if (data_->channel_)
  {
	const FMOD_VECTOR pos = { position.x, position.y, position.z };
	auto error = data_->channel_->set3DAttributes(&pos, 0);
	if (error != FMOD_OK)
		printf("Error set_position\n");
  }
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_velocity(glm::vec3 velocity)
{
  data_->velocity_ = velocity;
  
  if (data_->channel_)
  {
	const FMOD_VECTOR vel = { velocity.x, velocity.y, velocity.z };
	data_->channel_->set3DAttributes(0, &vel);
  }
	
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_looping(bool loop)
{
  if (data_->sound_)
  {
    if (loop)
    {
      data_->sound_->setMode(FMOD_LOOP_NORMAL);
      data_->sound_->setLoopCount(-1);
    }
    else
    {
      data_->sound_->setMode(FMOD_LOOP_OFF);
      data_->sound_->setLoopCount(1);
    }
  }

}

////////////////////////////////////////////////////////////////
bool BufferAudioSource::is_looping() const
{
  int loopCount = 0;

  if (data_->sound_)
  {
    data_->sound_->getLoopCount(&loopCount);
  }

  return loopCount == -1;
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_gain(float gain)
{

  if (data_->sound_ && data_->channel_)
  {
    data_->channel_->setVolume(gain);
  }

}

////////////////////////////////////////////////////////////////
float BufferAudioSource::gain() const
{
  float volume = 0.0f;

  if (data_->sound_ && data_->channel_)
  {
    data_->channel_->getVolume(&volume);
  }

  return volume;
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_pitch(float pitch)
{
  if (data_->channel_)
  {
    data_->channel_->setPitch(pitch);
  }

}

////////////////////////////////////////////////////////////////
float BufferAudioSource::pitch() const
{
  float pitch = 0.0f;

  if (data_->channel_)
  {
    data_->channel_->getPitch(&pitch);
  }

  return pitch;
}

////////////////////////////////////////////////////////////////
bool BufferAudioSource::is_playing() const
{
  bool isPlaying = false;

  if (data_->channel_)
  {
    data_->channel_->isPlaying(&isPlaying);
  }

  return isPlaying;
}

////////////////////////////////////////////////////////////////
void* BufferAudioSource::data() const
{
  return data_.get();
}

////////////////////////////////////////////////////////////////
void BufferAudioSource::set_buffer(const AudioBuffer& buffer)
{
  auto data = static_cast<AudioBuffer::Data*>(buffer.data());

  data_->sound_ = data->sound_;
  data_->channel_ = data_->channel_;
  if (data_->channel_)
  {
    data_->channel_->getIndex(&data_->channel_index_);
  }
}

}

#endif

