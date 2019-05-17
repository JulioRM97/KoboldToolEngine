#include "gltf_texture.h"
#include "kte/graphics/gl_internals.h"
#include "kte/graphics/impl/gltf/gltf_internals.h"
#include "kte/common.h"

#include <cstdlib>

#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "kte/game/game_manager.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void GLTFTexture::GPUData::load()
{
  $
  if (cpu_version_ <= gpu_version_)
  {
    return;
  }

  uint32_t next_version = cpu_version_;

  // Create texture id
  if (texture_id_ == 0)
  {
    glGenTextures(1, &texture_id_);
    checkErrors("glGenTextures");
  }

  // Bind texture
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  checkErrors("glBindTexture");

  // Load data
  if (image_)
  {
    GLenum size = image_->component == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, size, image_->width, image_->height, 0,
                 size, GL_UNSIGNED_BYTE, &image_->image.at(0));
    checkErrors("glTexImage2D");

    // Parameters
    gltfTexParameteri(GL_TEXTURE_MIN_FILTER, sampler_->minFilter);
    gltfTexParameteri(GL_TEXTURE_MAG_FILTER, sampler_->magFilter);
    gltfTexParameteri(GL_TEXTURE_WRAP_S, sampler_->wrapS);
    gltfTexParameteri(GL_TEXTURE_WRAP_T, sampler_->wrapT);
    gltfTexParameteri(GL_TEXTURE_WRAP_R, sampler_->wrapR);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    checkErrors("glGenerateMipmap");
  }
  else
  {
    static const glm::vec3 white(1.0f);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT,
                 static_cast<const void*>(glm::value_ptr(white)));
    checkErrors("glTexImage2D");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  }

  // Unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  checkErrors("glBindTexture");

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void GLTFTexture::GPUData::unload()
{
  if (texture_id_ != 0)
  {
    glDeleteTextures(1, &texture_id_);
  }
}

////////////////////////////////////////////////////////////////
GLTFTexture::GLTFTexture()
    : data_(new GPUData())
{
  // A default texture is just a blank texture that is ready to
  // be uploaded to the gpu.
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
GLTFTexture::GLTFTexture(gltf_shared_ptr<tinygltf::Texture> texture)
    : data_(new GPUData())
{
  auto model_data = texture.model_data();
  auto& model = model_data->gltf_model_;

  data_->sampler_.set(&model.samplers[texture->sampler], model_data);

  if (texture->source >= 0)
  {
    data_->image_.set(&model.images[texture->source], model_data);

    if (data_->image_->bufferView >= 0)
    {
      data_->buffer_view_.set(&model.bufferViews[data_->image_->bufferView],
                              model_data);

      if (data_->buffer_view_->buffer >= 0)
      {
        data_->buffer_.set(&model.buffers[data_->buffer_view_->buffer],
                           model_data);
      }
    }
  }

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
GLTFTexture::GLTFTexture(std::string file_name)
    : data_(new GPUData())
{
  data_->sampler_.alloc();
  {
    data_->sampler_->minFilter = GL_LINEAR_MIPMAP_LINEAR;
    data_->sampler_->magFilter = GL_LINEAR;
    data_->sampler_->wrapS = TINYGLTF_TEXTURE_WRAP_REPEAT;
    data_->sampler_->wrapT = TINYGLTF_TEXTURE_WRAP_REPEAT;
    data_->sampler_->wrapR = TINYGLTF_TEXTURE_WRAP_REPEAT;
  }

  data_->image_.alloc();
  {
    auto image = data_->image_.get();

    int force_channels = 4;
    unsigned char* image_data = stbi_load(file_name.c_str(),
                                          &image->width, &image->height,
                                          &image->component, force_channels);

    auto data_length =
        static_cast<size_t>(image->width * image->height * image->component);

    image->image.resize(data_length);
    std::copy(image_data, image_data + data_length, image->image.begin());

    free(image_data);
  }

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
GLTFTexture::~GLTFTexture()
{
  unload();
}

////////////////////////////////////////////////////////////////
void GLTFTexture::load()
{
  data_->load();
}

////////////////////////////////////////////////////////////////
void GLTFTexture::generateMipmap()
{
  if (data_->sampler_->minFilter != GL_NEAREST ||
      data_->sampler_->minFilter != GL_LINEAR ||
      data_->sampler_->magFilter != GL_NEAREST ||
      data_->sampler_->magFilter != GL_LINEAR)
  {
    glBindTexture(GL_TEXTURE_2D, internal_id());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    checkErrors("TextureAttachment::generateMipmap");
  }
}

////////////////////////////////////////////////////////////////
void GLTFTexture::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&GLTFTexture::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
unsigned int GLTFTexture::internal_id() const
{
  return data_->texture_id_;
}

////////////////////////////////////////////////////////////////
glm::uvec2 GLTFTexture::size() const
{
  return glm::uvec2(data_->image_->width, data_->image_->height);
}

} // end namespace graphics
} // end namespace kte
