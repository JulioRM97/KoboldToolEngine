#pragma once

#include <string>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace kte
{

/// Represents an audio listener, usually it's tied to the game camera.
class AudioListener
{
 public:
  AudioListener();

  ~AudioListener();

  /// \param position The new position of the listener.
  void set_position(glm::vec3 position);

  /// \param velocity The new velocity of the listener.
  void set_velocity(glm::vec3 velocity);

  /// \param orientation The new orientation of the listener.
  void set_orientation(glm::mat4 orientation);

  struct Data;

 private:
  std::unique_ptr<Data> data_;
};

}

