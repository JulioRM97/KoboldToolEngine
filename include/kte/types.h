#pragma once

#include <cstdint>

#include <glm/mat4x4.hpp>

namespace kte
{

class TransformComponent;

typedef uint16_t Key;
typedef uint8_t Button;

struct rect2i
{
  glm::uvec2 pos;
  glm::uvec2 size;
};

class CameraParameters
{
 public:
  virtual ~CameraParameters() = default;

  virtual const TransformComponent& transform() const = 0;

  virtual glm::mat4 projection() const = 0;

  virtual glm::mat4 view() const = 0;

  virtual rect2i viewport() const = 0;
};

namespace graphics
{

enum class RenderMode : uint8_t
{
  k_Forward,
  k_Deferred,
  k_ShadowMap,
  k_FullScreen,
  k_ZPrePass
};

} // end namespace graphics
} // end namespace kte

