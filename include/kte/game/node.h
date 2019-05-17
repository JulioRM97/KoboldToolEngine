#pragma once

#include <deque>
#include <memory>
#include <map>
#include <typeindex>

#include <glm/mat4x4.hpp>

#include "kte/game/scene.h"
#include "kte/game/components/transform_component.h"

namespace kte
{

// Forward declarations
class Scene;
class Component;

namespace graphics
{
class DisplayList;
}

/// Defines a node in a scene
class Node
{
 public:
  explicit Node(Scene* scene, Node* parent = nullptr);

  Node(const Node& other);

  ~Node();

  Node& addChild();

  /// Add a component by passing a shared pointer
  template<typename T>
  inline void addComponent(T* component)
  {
    components_.emplace(typeid(T), component);
  }

  /// Add a component by class
  template<typename T, typename... Args>
  T* createComponent(Args&& ... args)
  {
    auto& mm = scene_->getOrCreateSystem<T>()->memory_manager_;
    auto* parent_component = parent_ ? parent_->getComponent<T>() : nullptr;

    auto* t = &mm.emplace(parent_component, *this, std::forward<Args>(args)...);
    addComponent(t);
    return t;
  }

  /// \return true if there's a component of a class
  template<typename T>
  inline bool hasComponent() const { return components_.count(typeid(T)) > 0; }

  /// \return a component by class
  template<typename T>
  inline T* getComponent() const
  {
    return hasComponent<T>() ? static_cast<T*>(components_.at(typeid(T)))
                             : nullptr;
  }

  /// Get the transform component
  inline TransformComponent* transform() const
  {
    return getComponent<TransformComponent>();
  }

  // FIXME
  const inline std::deque<Component*> components() const
  {
    std::deque<Component*> components(components_.size());
    for (auto& comp : components_)
    {
      components.push_back(comp.second);
    }
    return components;
  }

  Scene* scene_ = nullptr;

  Node* parent_ = nullptr;

  std::deque<Node*> children_;

  std::map<std::type_index, Component*> components_;
};

} // end namespace kte

