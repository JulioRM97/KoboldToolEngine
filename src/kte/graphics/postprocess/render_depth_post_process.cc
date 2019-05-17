#include "kte/graphics/postprocess/render_depth_post_process.h"

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

RenderDepthPostProcess::RenderDepthPostProcess()
    : PostProcess("render_depth")
{
  passes_[0].input_framebuffer->addColorTextureAttachment();
  passes_[0].input_framebuffer->addDepthTextureAttachment();
}

RenderDepthPostProcess::~RenderDepthPostProcess() = default;

}
}