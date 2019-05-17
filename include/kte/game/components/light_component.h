#pragma once

#include "kte/game/component.h"
#include "kte/types.h"

#include <cstdint>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace kte
{

class TransformComponent;

namespace graphics
{
class Program;
struct ProgramInfo;
class Framebuffer;
}

enum class LightType : uint8_t
{
  k_None = 0,
  k_Ambient,
  k_Directional,
  k_Point,
  k_Spot,
  k_Emissive,
};

class LightComponent : public Component, public CameraParameters
{
 public:
  explicit LightComponent(Node& parent);

  ~LightComponent() override;

  void use(const kte::graphics::ProgramInfo& info) const;

  void update() override;

  void set_attenuation(float constant, float linear, float quadratic);

  void set_falloff(int exponent);

  void set_specular(float strenght = 1.0f, int density = 32);

  void set_color(glm::vec3 color);

  const TransformComponent& transform() const override;

  glm::mat4 projection() const override;

  glm::mat4 view() const override;

  rect2i viewport() const override;

  bool generate_shadow_map() const;

  void set_generate_shadow_map(bool generate_shadow_map);

  void set_intensity(float intensity);

  void set_type(LightType type);

  void set_direction(glm::vec3 dir);

  void editorSettings() override;

  std::shared_ptr<kte::graphics::Framebuffer> shadow_frame_buffer_ = nullptr;

 private:
  struct Data;

  std::unique_ptr<Data> data_;
};

} // end namespace kte

