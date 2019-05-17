#include "kte/audio/audio_manager.h"

#include <algorithm>

#include "kte/timer.h"
#include "kte/math.h"
#include "kte/audio/audio_source.h"

#ifdef KTE_AUDIO_LIB_FMOD
#include "impl/fmod_internals.h"
#endif

namespace kte
{

////////////////////////////////////////////////////////////////
const kte::AudioBuffer& AudioManager::getBuffer(std::string filename)
{
  if (buffers_.count(filename) == 0)
  {
    buffers_[filename].loadFromFile(filename);
  }

  return buffers_.at(filename);
}

////////////////////////////////////////////////////////////////
void AudioManager::update()
{
  for (auto it = fades_.cbegin(); it != fades_.cend();)
  {
    const auto fade = it->second;
    const auto speed = Time::deltaTime * fade.speed;
    const auto gain = kte::math::LerpTo(fade.source->gain(),
                                        fade.target_gain,
                                        speed);

    fade.source->set_gain(gain);

    if (gain == fade.target_gain)
    {
      it = fades_.erase(it);
    }
    else
    {
      ++it;
    }
  }

#ifdef KTE_AUDIO_LIB_FMOD
  gSystem->update();
#endif
}

////////////////////////////////////////////////////////////////
void AudioManager::fadeTo(std::shared_ptr<AudioSource> source,
                          float target_gain, float duration)
{
  auto& fade = fades_[source.get()];

  fade.source = source;
  fade.target_gain = target_gain;
  fade.speed = 1.0f / duration;
}

////////////////////////////////////////////////////////////////
void AudioManager::play(std::shared_ptr<AudioSource> source, int track)
{
  if (track >= 0)
  {
    if (tracks_.count(track) > 0)
    {
      auto& current_track = tracks_[track];
      current_track->stop();
    }

    tracks_[track] = source;
  }

  source->play();
  source->set_gain(1.0f);
}

////////////////////////////////////////////////////////////////
void AudioManager::playFadeIn(std::shared_ptr<AudioSource> source,
                              float duration,
                              int track)
{
  if (track >= 0)
  {
    if (tracks_.count(track) > 0)
    {
      auto& current_track = tracks_[track];
      current_track->stop();
    }

    tracks_[track] = source;
  }

  source->set_gain(0.0f);
  fadeTo(source, 1.0f, duration);
  source->play();
}

////////////////////////////////////////////////////////////////
void AudioManager::crossfadeTo(std::shared_ptr<AudioSource> source,
                               float duration,
                               int track)
{
  if (track >= 0)
  {
    if (tracks_.count(track) > 0)
    {
      auto& current_track = tracks_[track];
      fadeTo(current_track, 0.0f, duration);
    }

    tracks_[track] = source;
  }

  source->play();
  source->set_gain(0.0f);
  fadeTo(source, 1.0f, duration);
}

////////////////////////////////////////////////////////////////
void AudioManager::fadeOut(std::shared_ptr<AudioSource> source, float duration)
{
  fadeTo(source, 0.0f, duration);
}

////////////////////////////////////////////////////////////////
void AudioManager::fadeOut(float duration, int track)
{
  if (tracks_.count(track) > 0)
  {
    fadeTo(tracks_[track], 0.0f, duration);
    tracks_.erase(track);
  }
}

////////////////////////////////////////////////////////////////
void AudioManager::stop(std::shared_ptr<AudioSource> source)
{
  source->stop();
}

////////////////////////////////////////////////////////////////
void AudioManager::stop(int track)
{
  if (tracks_.count(track) > 0)
  {
    auto& current_track = tracks_[track];
    current_track->stop();
    tracks_.erase(track);
  }
}

} // namespace kte

