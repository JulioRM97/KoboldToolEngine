#include "kte/graphics/gl_internals.h"

#include "kte/graphics/framebuffer.h"
#include "kte/graphics/commands/copy_framebuffers_command.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
CopyFramebuffersCommand::CopyFramebuffersCommand(const std::shared_ptr<Framebuffer> framebuffer_from,
                                                 const std::shared_ptr<Framebuffer> framebuffer_to)
    : framebuffer_from_(framebuffer_from),
      framebuffer_to_(framebuffer_to)
{
}

////////////////////////////////////////////////////////////////
CopyFramebuffersCommand::~CopyFramebuffersCommand() = default;

////////////////////////////////////////////////////////////////
void CopyFramebuffersCommand::exec() const
{
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_from_->internal_id());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_to_->internal_id());

  glBlitFramebuffer(0, 0, framebuffer_from_->size().x, framebuffer_from_->size().y,
                    0, 0, framebuffer_to_->size().x, framebuffer_to_->size().y,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

} // end namespace graphics
} // end namespace kte

