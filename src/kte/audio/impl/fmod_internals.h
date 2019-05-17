#pragma once

#include "kte/audio/audio_listener.h"

#include <bitset>

#include <fmod.hpp>

#include "kte/audio/audio_buffer.h"
#include "kte/audio/buffer_audio_source.h"

#define NUM_MAX_CHANNELS 100
#define NUM_LISTENERS 1
#define DOPPLER_SCALE 1.0f
#define DISTANCE_FACTOR 1.0f
#define ROLL_OF_SCALE 1.0f

extern std::bitset<NUM_MAX_CHANNELS> gUsedChannels;

extern FMOD::System* gSystem;

namespace kte
{

////////////////////////////////////////////////////////////////
struct AudioListener::Data
{
  Data();

  ~Data() = default;

  FMOD_VECTOR position_;

  FMOD_VECTOR velocity_;

  FMOD_VECTOR forward_;

  FMOD_VECTOR up_;

};

////////////////////////////////////////////////////////////////
struct BufferAudioSource::Data
{
  Data() = default;

  FMOD::Sound* sound_ = nullptr;

  FMOD::Channel* channel_ = nullptr;

  glm::vec3 velocity_;

  glm::vec3 position_;

  int channel_index_ = 0;
};

////////////////////////////////////////////////////////////////
struct AudioBuffer::Data
{
  Data();

  void cleanup();

  FMOD::Sound* sound_ = nullptr;

  FMOD::Channel* channel_ = nullptr;
};

}


