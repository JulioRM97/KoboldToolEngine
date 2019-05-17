#include "kte/audio/layered_audio_source.h"

namespace kte
{

////////////////////////////////////////////////////////////////
kte::LayeredAudioSource::LayeredAudioSource() = default;

////////////////////////////////////////////////////////////////
kte::LayeredAudioSource::~LayeredAudioSource() = default;

////////////////////////////////////////////////////////////////
void LayeredAudioSource::play()
{
  for (const auto& layer : layers_)
  {
    layer->play();
  }
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::stop()
{
  for (const auto& layer : layers_)
  {
    layer->stop();
  }
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::set_position(glm::vec3 position)
{
  for (const auto& layer : layers_)
  {
    layer->set_position(position);
  }
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::set_velocity(glm::vec3 velocity)
{
  for (const auto& layer : layers_)
  {
    layer->set_velocity(velocity);
  }
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::set_looping(bool loop)
{
  for (const auto& layer : layers_)
  {
    layer->set_looping(loop);
  }
}

////////////////////////////////////////////////////////////////
bool LayeredAudioSource::is_looping() const
{
  for (const auto& layer : layers_)
  {
    if (layer->is_looping()) {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::set_gain(float gain)
{
  for (const auto& layer : layers_)
  {
    layer->set_gain(gain);
  }
}

////////////////////////////////////////////////////////////////
float LayeredAudioSource::gain() const
{
  float avg_gain = 0.0f;
  for (const auto& layer : layers_)
  {
    avg_gain += layer->gain() / layers_.size();
  }
  return avg_gain;
}

////////////////////////////////////////////////////////////////
void LayeredAudioSource::set_pitch(float pitch)
{
  for (const auto& layer : layers_)
  {
    layer->set_pitch(pitch);
  }
}

////////////////////////////////////////////////////////////////
float LayeredAudioSource::pitch() const
{
  float avg_pitch = 0.0f;
  for (const auto& layer : layers_)
  {
    avg_pitch += layer->pitch() / layers_.size();
  }
  return avg_pitch;
}

////////////////////////////////////////////////////////////////
bool LayeredAudioSource::is_playing() const
{
  for (const auto& layer : layers_)
  {
    if (layer->is_playing())
    {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////
void* LayeredAudioSource::data() const
{
  for (const auto& layer : layers_)
  {
    if (layer->is_playing())
    {
      return layer->data();
    }
  }
  return nullptr;
}

}





