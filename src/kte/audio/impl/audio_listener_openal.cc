#ifdef KTE_AUDIO_LIB_OPENAL

#include "kte/audio/audio_listener.h"

#include <AL/al.h>
#include <AL/alut.h>

namespace kte
{

////////////////////////////////////////////////////////////////
struct AudioListener::Data
{
};

////////////////////////////////////////////////////////////////
AudioListener::AudioListener() : data_(new Data())
{
  set_position(glm::vec3(0));
  set_velocity(glm::vec3(0));
  set_orientation(glm::mat4(1));
}

////////////////////////////////////////////////////////////////
AudioListener::~AudioListener() = default;

////////////////////////////////////////////////////////////////
void AudioListener::set_position(glm::vec3 position)
{
  alListener3f(AL_POSITION, position.x, position.y, position.z);
}

////////////////////////////////////////////////////////////////
void AudioListener::set_velocity(glm::vec3 velocity)
{
  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

////////////////////////////////////////////////////////////////
void AudioListener::set_orientation(glm::mat4 orientation)
{
  auto look_at = orientation[2];
  auto up = orientation[1];

  ALfloat orientation_[] = {
      look_at.x, look_at.y, look_at.z,
      up.x, up.y, up.z
  };

  alListenerfv(AL_ORIENTATION, orientation_);
}

}

#endif

