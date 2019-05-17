#include "kte/graphics/postprocess/base_render.h"

#include "kte/graphics/framebuffer.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
BaseRender::BaseRender()
    : PostProcess("base_render")
{
  passes_[0].input_framebuffer->addColorTextureAttachment();
  passes_[0].input_framebuffer->addDepthTextureAttachment();
}

////////////////////////////////////////////////////////////////
BaseRender::~BaseRender() = default;

} // end namespace graphics
} // end namespace kte

