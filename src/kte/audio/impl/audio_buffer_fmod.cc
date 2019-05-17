#ifdef KTE_AUDIO_LIB_FMOD

#include "kte/audio/audio_buffer.h"
#include "fmod_internals.h"

#include <algorithm>

namespace kte
{

////////////////////////////////////////////////////////////////
AudioBuffer::Data::Data()
{
}

////////////////////////////////////////////////////////////////
void AudioBuffer::Data::cleanup()
{
  if (sound_)
  {
    sound_->release();
  }
}

////////////////////////////////////////////////////////////////
AudioBuffer::AudioBuffer()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
AudioBuffer::~AudioBuffer()
{
  data_->cleanup();
}

////////////////////////////////////////////////////////////////
void AudioBuffer::loadFromFile(std::string filename)
{
  data_->cleanup();
  gSystem->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &data_->sound_);

  for (int i = 0; i < gUsedChannels.size(); i++)
  {
    if (!gUsedChannels[i])
    {
      gSystem->getChannel(i, &data_->channel_);
      return;
    }
  }

  gSystem->getChannel(0, &data_->channel_);
}

////////////////////////////////////////////////////////////////
void* AudioBuffer::data() const
{
  return static_cast<void*>(data_.get());
}

} // namespace kte

#endif

