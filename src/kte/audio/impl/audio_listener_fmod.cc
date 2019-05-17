#ifdef KTE_AUDIO_LIB_FMOD

#include "kte/audio/audio_listener.h"
#include "fmod_internals.h"

namespace kte
{

////////////////////////////////////////////////////////////////
AudioListener::Data::Data()
{
  position_ = {0.0f, 0.0f, 0.0f};
  velocity_ = {1.0f, 1.0f, 1.0f};
  forward_ = {1.0f, 0.0f, 0.0f};
  up_ = {0.0f, 1.0f, 0.0f};
}

////////////////////////////////////////////////////////////////
AudioListener::AudioListener()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
AudioListener::~AudioListener() = default;

////////////////////////////////////////////////////////////////
void AudioListener::set_position(glm::vec3 position)
{
  data_->position_ = {position.x, position.y, position.z};
  if (gSystem->set3DListenerAttributes(0, &data_->position_, &data_->velocity_, &data_->forward_, &data_->up_) != FMOD_OK)
	  printf("error set_position\n");
}

////////////////////////////////////////////////////////////////
void AudioListener::set_velocity(glm::vec3 velocity)
{
  data_->velocity_ = {velocity.x, velocity.y, velocity.z};
  if (gSystem->set3DListenerAttributes(0, &data_->position_, &data_->velocity_, &data_->forward_, &data_->up_) != FMOD_OK)
    printf("error set_velocity\n");

}

////////////////////////////////////////////////////////////////
void AudioListener::set_orientation(glm::mat4 orientation)
{
  auto look_at = orientation[2];
  auto up = orientation[1];

  data_->forward_ = { 0.0f, 0.0f, 1.0f};
  data_->up_ = {0.0, 1.0f, 0.0f};
 

 if (gSystem->set3DListenerAttributes(0, &data_->position_, &data_->velocity_, &data_->forward_, &data_->up_) != FMOD_OK)
	printf("error set_orientation\n");
}

}

#endif

