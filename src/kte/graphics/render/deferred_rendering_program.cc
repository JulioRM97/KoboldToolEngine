#include <random>
#include "deferred_rendering_program.h"
#include "kte/graphics/gl_internals.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
DeferredRenderingProgram::DeferredRenderingProgram()
    : FullScreenProgram("deferred_rendering"),
      g_buffer_(std::make_shared<Framebuffer>())
{
  data_->required_attributes = {{"position", "a_position"},
                                {"texCoord", "a_texCoord"}};

  // TODO see FullScreenProgram::setupInputFramebuffer
  data_->required_textures = {"baseColor", "emissiveColor", "normal", "position"};

  data_->required_uniforms = {{"noise", "noise_tex"},
                              {"PROJECTION", "u_m_projection"},
                              {"samples", "samples"}};

  // baseColor, emissiveColor, position, normal & depth
  g_buffer_->addColorTextureAttachment();
  g_buffer_->addColorTextureAttachment();
  g_buffer_->addDataTextureAttachment();
  g_buffer_->addDataTextureAttachment();
  g_buffer_->addDepthTextureAttachment();

  std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
  std::default_random_engine generator;

  for (unsigned int i = 0; i < 64; ++i)
  {
    glm::vec3 sample(
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator)
    );
    sample  = glm::normalize(sample);
    sample *= randomFloats(generator);
    float scale = (float)i / 64.0f;
    scale   = glm::mix(0.1f, 1.0f, scale * scale);
    sample *= scale;

    ssao_kernel_.push_back(sample);
  }

  for (unsigned int i = 0; i < 16; i++)
  {
    glm::vec3 noise(
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator) * 2.0 - 1.0,
        0.0f);
    ssao_noise_.push_back(noise);
  }

  noise_tex_ = std::make_shared<DataTexture>(&ssao_noise_[0], glm::ivec2(4, 4));
}

////////////////////////////////////////////////////////////////
DeferredRenderingProgram::~DeferredRenderingProgram() = default;

void DeferredRenderingProgram::load()
{
  FullScreenProgram::load();

  noise_tex_->load();
}
} // end namespace graphics
} // end namespace kte

