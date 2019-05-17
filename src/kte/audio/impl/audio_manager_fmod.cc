#ifdef KTE_AUDIO_LIB_FMOD

#include "kte/audio/audio_manager.h"
#include "fmod_internals.h"

FMOD::System* gSystem = nullptr;

std::bitset<NUM_MAX_CHANNELS> gUsedChannels;

namespace kte
{

////////////////////////////////////////////////////////////////
struct AudioManager::Data
{
  Data();

  void init();

  void cleanup();

};

////////////////////////////////////////////////////////////////
AudioManager::Data::Data()
{
  init();
}

////////////////////////////////////////////////////////////////
void AudioManager::Data::init()
{
  FMOD::System_Create(&gSystem);
  gSystem->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, NULL);

  if (gSystem->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLL_OF_SCALE) != FMOD_OK)
	  printf("set3DSettings");

}

////////////////////////////////////////////////////////////////
void AudioManager::Data::cleanup()
{
  gSystem->release();
}

////////////////////////////////////////////////////////////////
AudioManager::AudioManager()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
AudioManager::~AudioManager() = default;

} // namespace kte

#endif

