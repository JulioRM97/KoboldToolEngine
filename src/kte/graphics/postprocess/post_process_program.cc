#include "post_process_program.h"
#include "kte/graphics/gl_internals.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
PostProcessProgram::PostProcessProgram(std::string base_shader_name)
    : FullScreenProgram(base_shader_name)
{
  // TODO see FullScreenProgram::setupInputFramebuffer
  data_->required_textures = {"screenTexture", "depthTexture"};
}

////////////////////////////////////////////////////////////////
PostProcessProgram::~PostProcessProgram() = default;

} // end namespace graphics
} // end namespace kte

