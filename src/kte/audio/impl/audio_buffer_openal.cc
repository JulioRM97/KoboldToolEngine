#ifdef KTE_AUDIO_LIB_OPENAL

#include "kte/audio/audio_buffer.h"

#include <AL/al.h>
#include <AL/alut.h>

#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

namespace kte
{

////////////////////////////////////////////////////////////////
struct AudioBuffer::Data
{
  void cleanup();

  ALuint buffer_ = 0;
};

////////////////////////////////////////////////////////////////
void AudioBuffer::Data::cleanup()
{
  if (buffer_)
  {
    alDeleteBuffers(1, &buffer_);
    buffer_ = 0;
  }
}

////////////////////////////////////////////////////////////////
AudioBuffer::AudioBuffer() : data_(new Data())
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

  int channels;
  int sample_rate;
  short *output;
  int samples = stb_vorbis_decode_filename(filename.c_str(), &channels, &sample_rate, &output);

  if (samples == -1)
  {
    fprintf(stderr, "Cannot read %s\n", filename.c_str());
    return;
  }

  alGenBuffers(1, &data_->buffer_);
  alBufferData(data_->buffer_,
               channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
               output, channels * samples * sizeof(short), sample_rate);

  auto error = alutGetError();

  if (error)
  {
    fprintf(stderr, "Error reading %s: %s\n", filename.c_str(), alutGetErrorString(error));
    return;
  }

  free(output);
}

////////////////////////////////////////////////////////////////
void* AudioBuffer::data() const
{
  return static_cast<void*>(&data_->buffer_);
}

}

#endif

