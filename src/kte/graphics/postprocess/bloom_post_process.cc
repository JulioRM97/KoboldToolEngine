#include "kte/graphics/postprocess/bloom_post_process.h"

#include "kte/graphics/framebuffer.h"
#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/commands/generate_mipmap_command.h"
#include "kte/graphics/display/display_list.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
BloomPostProcess::BloomPostProcess()
    : PostProcess("bloom")
{
  passes_[0].input_framebuffer->addColorTextureAttachment(true);
  passes_[0].input_framebuffer->addDepthTextureAttachment();
}

////////////////////////////////////////////////////////////////
BloomPostProcess::~BloomPostProcess() = default;

////////////////////////////////////////////////////////////////
void BloomPostProcess::renderPass(const std::shared_ptr<Framebuffer> output,
                                  const LightComponent* light,
                                  const PostProcessPass& pass,
                                  DisplayList& list) const
{
  list.add<GenerateMipmapCommand>(pass.input_framebuffer->color_texture());

  PostProcess::renderPass(output, light, pass, list);
}

} // end namespace graphics
} // end namespace kte

