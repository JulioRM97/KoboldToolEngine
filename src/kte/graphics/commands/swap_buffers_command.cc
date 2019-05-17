#include "kte/graphics/commands/swap_buffers_command.h"
#include "kte/graphics/vr/vr_common.h"

#include "kte/game/game_manager.h"
#include "kte/graphics/render_manager.h"
#include "kte/graphics/window.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
SwapBuffersCommand::SwapBuffersCommand() = default;

////////////////////////////////////////////////////////////////
SwapBuffersCommand::~SwapBuffersCommand() = default;

////////////////////////////////////////////////////////////////
void SwapBuffersCommand::exec() const
{
  const auto& manager = GameManager::Instance();
  auto& render_manager = manager.render_manager_;

  if (render_manager.use_vr_)
  {
#if OCULUS
    auto& oculus_data = const_cast<OculusHMD&>(render_manager.oculus_hmd_);
    oculus_data.submitFrame(render_manager.frame_);
#endif
  }

  GameManager::Instance().window_.swap();

  render_manager.frame_++;
}

} // end namespace graphics
} // end namespace kte

