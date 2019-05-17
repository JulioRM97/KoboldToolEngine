#include "kte/game/components/light_component.h"

#include <glm/gtc/constants.hpp>

#include <kte/game/game_manager.h>
#include <kte/game/components/light_component.h>

#include "kte/game/components/transform_component.h"
#include "kte/game/components/camera_component.h"
#include "kte/game/node.h"
#include "kte/graphics/gl_internals.h"
#include "kte/graphics/program.h"
#include "kte/graphics/framebuffer.h"

namespace kte
{

////////////////////////////////////////////////////////////////
struct LightComponent::Data
{
  void use(const kte::graphics::ProgramInfo& info) const;

  LightType type = LightType::k_Ambient;

  glm::vec3 position{1.0f};

  glm::vec3 direction{1.0f, 0.0f, 0.0f};

  glm::vec3 color{1.0f};

  float intensity = 2.0f;

  float constant_attenuation = 1.0f;

  float linear_attenuation = 0.0f;

  float quadratic_attenuation = 0.0f;

  float emissive_factor = 0.0f;

  int falloff_exponent = 0;

  float specular_strength = 1.0f;

  int specular_density = 32;

  glm::mat4 projection;

  glm::mat4 view;

  std::shared_ptr<kte::graphics::TextureAttachment> shadow_tex;

  bool generate_shadow_map = true;
};

////////////////////////////////////////////////////////////////
void LightComponent::Data::use(const kte::graphics::ProgramInfo& info) const
{
  glUseProgram(info.program_id);

  const auto camera = kte::GameManager::Instance().current_camera_;
  auto cam_pos = camera->transform().world_translation();
  glUniform3fv(info.light_uniforms.camera_position, 1, glm::value_ptr(cam_pos));
  checkErrors("glUniform3fv light.camera_position");

  glUniform3fv(info.light_uniforms.position, 1, glm::value_ptr(position));
  checkErrors("glUniform3fv light.position");

  glUniform3fv(info.light_uniforms.direction, 1, glm::value_ptr(direction));
  checkErrors("glUniform3fv light.direction");

  glUniform3fv(info.light_uniforms.color, 1, glm::value_ptr(color));
  checkErrors("glUniform3fv light.color");

  glUniform1f(info.light_uniforms.intensity, intensity);
  checkErrors("glUniform1f light.intensity");

  glUniform1f(info.light_uniforms.constant_attenuation, constant_attenuation);
  checkErrors("glUniform1f light.constant_attenuation");

  glUniform1f(info.light_uniforms.linear_attenuation, linear_attenuation);
  checkErrors("glUniform1f light.linear_attenuation");

  glUniform1f(info.light_uniforms.quadratic_attenuation, quadratic_attenuation);
  checkErrors("glUniform1f light.quadratic_attenuation");

  glUniform1f(info.light_uniforms.emissive_factor, emissive_factor);
  checkErrors("glUniform1f light.emissive_factor");

  glUniform1i(info.light_uniforms.falloff_exponent, falloff_exponent);
  checkErrors("glUniform1f light.falloff_exponent");

  glUniform1f(info.light_uniforms.specular_strength, specular_strength);
  checkErrors("glUniform1i light.specular_strenght");

  glUniform1i(info.light_uniforms.specular_density, specular_density);
  checkErrors("glUniform1f light.specular_density");

  glUniformMatrix4fv(info.light_uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
  checkErrors("glUniform1f light.projection");

  glUniformMatrix4fv(info.light_uniforms.view, 1, GL_FALSE, glm::value_ptr(view));
  checkErrors("glUniform1f light.view");

  glActiveTexture(GL_TEXTURE31);
  glBindTexture(GL_TEXTURE_2D, shadow_tex->internal_id());
  glUniform1i(info.light_uniforms.shadow_tex, 31);
  checkErrors("glUniform1f light.shadow_tex");

  glUseProgram(0);
}

////////////////////////////////////////////////////////////////
LightComponent::LightComponent(Node& parent)
    : Component(parent),
      shadow_frame_buffer_(std::make_shared<kte::graphics::Framebuffer>()),
      data_(new Data())
{
  shadow_frame_buffer_->set_size(glm::ivec2(640, 480));
  shadow_frame_buffer_->addDepthTextureAttachment();

  data_->shadow_tex = shadow_frame_buffer_->depth_texture();
}

////////////////////////////////////////////////////////////////
LightComponent::~LightComponent() = default;

////////////////////////////////////////////////////////////////
void LightComponent::use(const kte::graphics::ProgramInfo& info) const
{
  data_->use(info);
}

////////////////////////////////////////////////////////////////
void LightComponent::update()
{
  data_->position = owner_node_.transform()->world_translation();
  data_->direction = owner_node_.transform()->forward();

  data_->view = glm::inverse(owner_node_.transform()->world_transform());
}

////////////////////////////////////////////////////////////////
void LightComponent::set_attenuation(float constant,
                                     float linear,
                                     float quadratic)
{
  data_->constant_attenuation = constant;
  data_->linear_attenuation = linear;
  data_->quadratic_attenuation = quadratic;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_falloff(int falloff_exponent)
{
  data_->falloff_exponent = falloff_exponent;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_specular(float strength, int density)
{
  data_->specular_strength = strength;
  data_->specular_density = density;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_color(glm::vec3 color)
{
  data_->color = color;
}

////////////////////////////////////////////////////////////////
const TransformComponent& LightComponent::transform() const
{
  return *owner_node_.transform();
}

////////////////////////////////////////////////////////////////
glm::mat4 LightComponent::projection() const
{
  return data_->projection;
}

////////////////////////////////////////////////////////////////
glm::mat4 LightComponent::view() const
{
  return data_->view;
}

////////////////////////////////////////////////////////////////
rect2i LightComponent::viewport() const
{
  return rect2i();
}

////////////////////////////////////////////////////////////////
bool LightComponent::generate_shadow_map() const
{
  return data_->generate_shadow_map;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_generate_shadow_map(bool generate_shadow_map)
{
  data_->generate_shadow_map = generate_shadow_map;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_intensity(float intensity)
{
  data_->intensity = intensity;
}

////////////////////////////////////////////////////////////////
void LightComponent::set_type(LightType type)
{
  data_->type = type;

  // Reset some light settings
  data_->generate_shadow_map = false;
  data_->constant_attenuation = 1.0f;
  data_->linear_attenuation = 0.0f;
  data_->quadratic_attenuation = 0.0f;
  data_->emissive_factor = 0.0f;

  switch (type)
  {
    case LightType::k_None:
    {
      break;
    }

    case LightType::k_Ambient:
    {
      break;
    }

    case LightType::k_Directional:
    {
      data_->specular_strength = 1.0f;
      data_->specular_density = 32;
      data_->generate_shadow_map = true;
      data_->projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 5.0f);
      break;
    }

    case LightType::k_Point:
    {
      data_->linear_attenuation = 0.25f;
      data_->quadratic_attenuation = 0.05f;
      break;
    }

    case LightType::k_Spot:
    {
      data_->specular_strength = 1.0f;
      data_->specular_density = 32;
      data_->generate_shadow_map = true;
      data_->projection = glm::perspective(0.75f, 1.5f, 0.1f, 30.0f);
      break;
    }

    case LightType::k_Emissive:
    {
      data_->emissive_factor = 1.0f;
      break;
    }
  }
}

void LightComponent::set_direction(glm::vec3 dir)
{
  data_->direction = dir;
}

} // end namespace kte

