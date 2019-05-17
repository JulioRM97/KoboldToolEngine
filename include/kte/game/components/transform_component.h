#pragma once

#include "kte/game/component.h"

#include <memory>

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace kte
{

class TransformComponent : public Component
{
 public:
  explicit TransformComponent(Node& parent);

  ~TransformComponent() override;

  void preUpdate() override;

  void update() override;

  void postUpdate() override;

  const glm::mat4 local_transform() const;

  const glm::mat4 world_transform() const;

  const glm::vec3 local_translation() const;

  const glm::vec3 world_translation() const;

  const glm::quat local_rotation() const;

  const glm::quat world_rotation() const;

  const glm::vec3 local_scale() const;

  const glm::vec3 world_scale() const;

  const glm::vec3 forward() const;

  const glm::vec3 right() const;

  const glm::vec3 up() const;

  void set_local_transform(glm::mat4 transform);

  void set_local_translation(glm::vec3 translation);

  void set_local_rotation(glm::quat rotation);

  void set_local_scale(glm::vec3 scale);

 private:
  void editorSettings() override;

  struct Data;

  std::unique_ptr<Data> data_;

};

} // end namespace kte

