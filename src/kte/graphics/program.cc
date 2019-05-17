#include "kte/graphics/program.h"
#include "kte/graphics/gl_internals.h"
#include "kte/common.h"

#include <map>

namespace kte
{
namespace graphics
{

static const std::map<RenderMode, std::string> gTypeNames{
    {RenderMode::k_Forward, "forward_"},
    {RenderMode::k_Deferred, "deferred_"},
    {RenderMode::k_ShadowMap, "depth_"},
    {RenderMode::k_ZPrePass, "depth_"},
    {RenderMode::k_FullScreen, ""}
};
////////////////////////////////////////////////////////////////
void Program::GPUData::load()
{ $
  if (cpu_version_ <= gpu_version_)
  {
    return;
  }

  uint32_t next_version = cpu_version_;

  // Load vertex shader shared by all shaders in this program
  GLuint vertex_shader_id = 0;
  if (!loadShaderFile(GL_VERTEX_SHADER, vertex_shader_id,
                      base_shader_name + "_vertex_shader.glsl"))
  {
    return;
  }

  // Load one fragment shader for each supported render mode
  for (auto& pair : programs)
  {
    load_mode(vertex_shader_id, pair.first);
  }

  // Cache attributes locations for later use, use any program info (first)
  if (!programs.empty())
  {
    auto& info = programs.begin()->second;
    for (const auto& pair : required_attributes)
    {
      attributes[pair.first] =
          glGetAttribLocation(info.program_id, pair.second.c_str());
    }
  }

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Program::GPUData::load_mode(GLuint vertex_shader_id, RenderMode mode)
{
  auto& info = programs[mode];

  if (info.program_id == 0)
  {
    info.program_id = glCreateProgram();
  }

  // Load fragment shader and link with the passed vertex shader
  GLuint fragment_shader_id = 0;
  auto filename = base_shader_name + "_fragment_" + gTypeNames.at(mode) + "shader.glsl";
  if (!loadShaderFile(GL_FRAGMENT_SHADER, fragment_shader_id, filename))
  {
    return;
  }

  if (!linkShaders(info.program_id, vertex_shader_id, fragment_shader_id))
  {
    fprintf(stderr, "Cannot link shaders for %s\n", base_shader_name.c_str());
    return;
  }

  // Cache uniform locations for later use
  for (const auto& pair : required_uniforms)
  {
    info.uniforms[pair.first] =
        glGetUniformLocation(info.program_id, pair.second.c_str());
  }

  // Cache texture uniform locations for later use
  for (const auto& texture_name : required_textures)
  {
    info.texture_uniforms[texture_name].texture =
        glGetUniformLocation(info.program_id, (texture_name + ".texture").c_str());
    info.texture_uniforms[texture_name].factor =
        glGetUniformLocation(info.program_id, (texture_name + ".factor").c_str());
    info.texture_uniforms[texture_name].tex_coord =
        glGetUniformLocation(info.program_id, (texture_name + ".texCoord").c_str());
  }

  // Cache factors uniform locations for later use
  {
    info.light_uniforms.position =
        glGetUniformLocation(info.program_id, "light.position");
    info.light_uniforms.direction =
        glGetUniformLocation(info.program_id, "light.direction");
    info.light_uniforms.color =
        glGetUniformLocation(info.program_id, "light.color");
    info.light_uniforms.intensity =
        glGetUniformLocation(info.program_id, "light.intensity");
    info.light_uniforms.constant_attenuation =
        glGetUniformLocation(info.program_id, "light.constant_attenuation");
    info.light_uniforms.linear_attenuation =
        glGetUniformLocation(info.program_id, "light.linear_attenuation");
    info.light_uniforms.quadratic_attenuation =
        glGetUniformLocation(info.program_id, "light.quadratic_attenuation");
    info.light_uniforms.emissive_factor =
        glGetUniformLocation(info.program_id, "light.emissive_factor");
    info.light_uniforms.falloff_exponent =
        glGetUniformLocation(info.program_id, "light.falloff_exponent");
    info.light_uniforms.specular_strength =
        glGetUniformLocation(info.program_id, "light.specular_strength");
    info.light_uniforms.specular_density =
        glGetUniformLocation(info.program_id, "light.specular_density");
    info.light_uniforms.camera_position =
        glGetUniformLocation(info.program_id, "light.camera_position");
    info.light_uniforms.projection =
        glGetUniformLocation(info.program_id, "light.projection");
    info.light_uniforms.view =
        glGetUniformLocation(info.program_id, "light.view");
    info.light_uniforms.shadow_tex =
        glGetUniformLocation(info.program_id, "light.shadow_tex");
  }

  for (const auto& factor_name : required_factors)
  {
    info.uniforms[factor_name] =
        glGetUniformLocation(info.program_id, factor_name.c_str());
  }
}

////////////////////////////////////////////////////////////////
void Program::GPUData::unload()
{
  for (auto& pair : programs)
  {
    glDeleteProgram(pair.second.program_id);
    pair.second.program_id = 0;
  }
}

////////////////////////////////////////////////////////////////
bool Program::supports(RenderMode mode) const
{
  return data_->programs.count(mode) > 0;
}

////////////////////////////////////////////////////////////////
Program::Program()
    : data_(std::make_shared<GPUData>())
{
}

////////////////////////////////////////////////////////////////
Program::Program(GPUData* data)
    : data_(data)
{
}

////////////////////////////////////////////////////////////////
Program::~Program() = default;

} // end namespace graphics
} // end namespace kte

