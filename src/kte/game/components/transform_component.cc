#define GLM_ENABLE_EXPERIMENTAL

#include "kte/game/components/transform_component.h"

#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "kte/game/node.h"

namespace kte
{

////////////////////////////////////////////////////////////////
struct TransformData
{
  void updateTransform();

  glm::vec3 translation{0};

  glm::quat rotation{};

  glm::vec3 scale{1};

  glm::mat4 transform{1};
};

////////////////////////////////////////////////////////////////
struct TransformComponent::Data
{
  void calculateNextWorldTransform(const Node& owner_node_);

  // TODO extract this to a "Transform System"
  TransformData previous_local;

  TransformData local;

  TransformData next_world;

  TransformData world;
};

////////////////////////////////////////////////////////////////
void TransformData::updateTransform()
{
  transform = glm::mat4(1);
  transform = glm::translate(transform, translation);
  transform *= glm::mat4_cast(rotation);
  transform = glm::scale(transform, scale);
}

////////////////////////////////////////////////////////////////
void TransformComponent::Data::calculateNextWorldTransform(const Node& owner_node_)
{
  if (owner_node_.parent_)
  {
    const auto parent_transform = owner_node_.parent_->transform();
    const auto& parent_next_world = parent_transform->data_->next_world;

    next_world.transform = parent_next_world.transform * previous_local.transform;
    next_world.translation = parent_next_world.transform * glm::vec4(previous_local.translation, 1.0f);
    next_world.rotation = glm::quat_cast(parent_next_world.transform) * previous_local.rotation;
    next_world.scale = parent_next_world.transform * glm::vec4(previous_local.scale, 1.0f);
  }
  else
  {
    next_world.transform = previous_local.transform;
    next_world.translation = previous_local.translation;
    next_world.rotation = previous_local.rotation;
    next_world.scale = previous_local.scale;
  }
}

////////////////////////////////////////////////////////////////
TransformComponent::TransformComponent(Node& parent)
    : Component(parent),
      data_(new Data())
{
}

////////////////////////////////////////////////////////////////
TransformComponent::~TransformComponent() = default;

////////////////////////////////////////////////////////////////
void TransformComponent::preUpdate()
{
  data_->previous_local = data_->local;
}

////////////////////////////////////////////////////////////////
void TransformComponent::update()
{
  data_->calculateNextWorldTransform(owner_node_);
}

////////////////////////////////////////////////////////////////
void TransformComponent::postUpdate()
{
  data_->world = data_->next_world;
}

////////////////////////////////////////////////////////////////
const glm::mat4 TransformComponent::local_transform() const
{
  return data_->local.transform;
}

////////////////////////////////////////////////////////////////
const glm::mat4 TransformComponent::world_transform() const
{
  return data_->world.transform;
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::local_translation() const
{
  return data_->local.translation;
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::world_translation() const
{
  return data_->world.translation;
}

////////////////////////////////////////////////////////////////
const glm::quat TransformComponent::local_rotation() const
{
  return data_->local.rotation;
}

////////////////////////////////////////////////////////////////
const glm::quat TransformComponent::world_rotation() const
{
  return data_->world.rotation;
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::local_scale() const
{
  return data_->local.scale;
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::world_scale() const
{
  return data_->world.scale;
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::forward() const
{
  return glm::normalize(glm::vec3(world_transform()[2]));
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::right() const
{
  return glm::normalize(glm::vec3(world_transform()[0]));
}

////////////////////////////////////////////////////////////////
const glm::vec3 TransformComponent::up() const
{
  return glm::normalize(glm::vec3(world_transform()[1]));
}

////////////////////////////////////////////////////////////////
void TransformComponent::set_local_transform(glm::mat4 transform)
{
    data_->local.transform = transform;

    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(data_->local.transform,
                   data_->local.scale,
                   data_->local.rotation,
                   data_->local.translation,
                   skew, perspective);

    data_->local.updateTransform();
}

////////////////////////////////////////////////////////////////
void TransformComponent::set_local_translation(glm::vec3 translation)
{
  data_->local.translation = translation;
  data_->local.updateTransform();
}

////////////////////////////////////////////////////////////////
void TransformComponent::set_local_rotation(glm::quat rotation)
{
  data_->local.rotation = rotation;
  data_->local.updateTransform();
}

////////////////////////////////////////////////////////////////
void TransformComponent::set_local_scale(glm::vec3 scale)
{
  data_->local.scale = scale;
  data_->local.updateTransform();
}

} // end namespace kte

