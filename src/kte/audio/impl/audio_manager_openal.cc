#ifdef KTE_AUDIO_LIB_OPENAL

#include "kte/audio/audio_manager.h"

#include <AL/alut.h>

namespace kte
{

////////////////////////////////////////////////////////////////
struct AudioManager::Data
{
  ALboolean init;
};

////////////////////////////////////////////////////////////////
AudioManager::AudioManager()
    : data_(new Data())
{
  data_->init = alutInit(nullptr, nullptr);
}

////////////////////////////////////////////////////////////////
AudioManager::~AudioManager()
{
  if (data_->init)
  {
    //alutExit();
  }
}

} // namespace kte

#endif

