#include "kte/game/components/audio_listener_component.h"

#include "kte/audio/audio_manager.h"
#include "kte/game/game_manager.h"
#include "kte/game/node.h"

namespace kte
{

////////////////////////////////////////////////////////////////
AudioListenerComponent::AudioListenerComponent(Node& parent)
    : Component(parent)
{
}

////////////////////////////////////////////////////////////////
AudioListenerComponent::~AudioListenerComponent() = default;

////////////////////////////////////////////////////////////////
void AudioListenerComponent::update()
{
  if (active_)
  {
    auto& listener = GameManager::Instance().audio_manager_.audio_listener_;
    auto transform = owner_node_.transform();

    listener.set_position(transform->world_translation());
    listener.set_orientation(transform->world_transform());
  }
}

} // end namespace kte

