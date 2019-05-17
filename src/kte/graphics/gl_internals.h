#pragma once

#include <atomic>
#include <map>
#include <deque>

#include <glad/glad.h>
#include <glm/vec4.hpp>

#include "kte/graphics/buffer.h"
#include "kte/graphics/framebuffer.h"
#include "kte/graphics/post_process.h"
#include "kte/graphics/program.h"
#include "kte/graphics/data_texture.h"
#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/drawables/sprite.h"
#include "kte/graphics/drawables/primitive.h"
#include "kte/graphics/drawables/skybox.h"
#include "kte/graphics/impl/gltf/gltf.h"
#include "kte/graphics/impl/gltf/gltf_texture.h"

#define checkErrors(desc) kte::graphics::_checkErrors(desc, __FILE__, __LINE__)
#define checkFrameBufferErrors(desc) kte::graphics::_checkFrameBufferErrors(desc, __FILE__, __LINE__)

namespace kte
{
namespace graphics
{

/// Checks errors in open gl
void _checkErrors(std::string desc, const char* file, int line);

void _checkFrameBufferErrors(std::string desc, const char* file, int line);

/// Load a file
std::string loadTextFile(const std::string& filename);

/// Load a shader from a file
bool loadShaderFile(GLenum shaderType, GLuint& shader, const std::string& filename);

/// Load a shader
bool loadShader(GLenum shaderType, GLuint& shader, const std::string& src);

/// Link shaders and generate a program
bool linkShaders(GLuint& program_id,
                 GLuint& vertex_shader_id,
                 GLuint& fragment_shader_id);

////////////////////////////////////////////////////////////////
struct GPUDataBase
{
  virtual void load() = 0;

  virtual void unload() = 0;

  std::atomic_uint cpu_version_{0};

  uint32_t gpu_version_ = 0;
};

////////////////////////////////////////////////////////////////
struct TextureUniforms
{
  GLint texture = -1;

  GLint factor = -1;

  GLint tex_coord = -1;
};

////////////////////////////////////////////////////////////////
struct LightUniforms
{
  GLint position = -1;

  GLint direction = -1;

  GLint color = -1;

  GLint intensity = -1;

  GLint constant_attenuation = -1;

  GLint linear_attenuation = -1;

  GLint quadratic_attenuation = -1;

  GLint emissive_factor = -1;

  GLint falloff_exponent = -1;

  GLint specular_strength = -1;

  GLint specular_density = -1;

  GLint camera_position = -1;

  GLint projection = -1;

  GLint view = -1;

  GLint shadow_tex = -1;
};

////////////////////////////////////////////////////////////////
struct ProgramInfo
{
  GLuint program_id = 0;

  std::map<std::string, GLint> uniforms;

  std::map<std::string, TextureUniforms> texture_uniforms;

  LightUniforms light_uniforms;
};

////////////////////////////////////////////////////////////////
struct Program::GPUData : public GPUDataBase
{
  void load() override;

  void load_mode(GLuint vertex_shader_id, RenderMode mode);

  void unload() override;

  std::string base_shader_name;

  std::map<RenderMode, ProgramInfo> programs;

  std::map<std::string, GLint> attributes;

  std::vector<std::string> required_textures;

  std::vector<std::string> required_factors;

  std::map<std::string, std::string> required_attributes;

  std::map<std::string, std::string> required_uniforms;
};

////////////////////////////////////////////////////////////////
struct Buffer::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint buffer_id_ = 0;

  GLenum target_ = GL_INVALID_ENUM;

  const void* buffer_data_ = nullptr;

  GLsizeiptr data_length_ = 0;

  GLsizei byte_stride_ = 0;
};

////////////////////////////////////////////////////////////////
struct Primitive::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint vao_id_ = 0;

  std::shared_ptr<Material> material_;

  std::map<int, std::shared_ptr<kte::graphics::Buffer>> buffers_;

  // TODO this is not technically gl data, but gltf model data. Do something.
  gltf_shared_ptr<tinygltf::Primitive> primitive_;

  std::map<int, gltf_shared_ptr<tinygltf::Accessor>> accessors_;
};

////////////////////////////////////////////////////////////////
struct Sprite::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint vao_id = 0;
};

////////////////////////////////////////////////////////////////
struct GLTFTexture::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint texture_id_ = 0;

  gltf_shared_ptr<tinygltf::Sampler> sampler_;

  gltf_shared_ptr<tinygltf::Image> image_;

  gltf_shared_ptr<tinygltf::BufferView> buffer_view_;

  gltf_shared_ptr<tinygltf::Buffer> buffer_;
};

////////////////////////////////////////////////////////////////
struct TextureAttachment::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  TextureAttachment::Type type = TextureAttachment::Type::k_None;

  GLuint texture_id = 0;

  uint8_t color_idx = 0;

  bool generate_mipmap = false;

  bool init_texture = true;

  glm::uvec2 size{0};
};

////////////////////////////////////////////////////////////////
struct DataTexture::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint texture_id = 0;

  uint8_t color_idx = 0;

  bool generate_mipmap = false;

  void* data = nullptr;

  glm::uvec2 size{0};
};

////////////////////////////////////////////////////////////////
struct Framebuffer::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  GLuint framebuffer_id = 0;

  GLbitfield clear_flags = 0;

  std::vector<GLuint> attachments;

  std::deque<std::shared_ptr<TextureAttachment>> color_textures;

  std::shared_ptr<TextureAttachment> depth_texture;

  std::shared_ptr<TextureAttachment> stencil_texture;

  bool srgb = false;
};

////////////////////////////////////////////////////////////////
struct Skybox::GPUData : public GPUDataBase
{
  void load() override;

  void unload() override;

  std::string nx;

  std::string ny;

  std::string nz;

  std::string px;

  std::string py;

  std::string pz;

  GLuint texture_id = 0;

  GLuint vao_id = 0;

  GLuint vbo_id = 0;
};

} // end namespace graphics
} // end namespace kte

