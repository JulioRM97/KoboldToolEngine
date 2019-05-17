#include "kte/game/components/audio_source_component.h"

#include "kte/audio/audio_source.h"
#include "kte/game/node.h"

namespace kte
{

////////////////////////////////////////////////////////////////
AudioSourceComponent::AudioSourceComponent(Node& parent)
    : Component(parent)
{
}

////////////////////////////////////////////////////////////////
AudioSourceComponent::~AudioSourceComponent() = default;

////////////////////////////////////////////////////////////////
void AudioSourceComponent::update()
{
  if (audio_source_)
  {
    audio_source_->set_position(owner_node_.transform()->world_translation());
  }
}

} // end namespace kte

