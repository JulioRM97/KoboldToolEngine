#include "kte/graphics/commands/start_frame_command.h"
#include "kte/graphics/vr/vr_common.h"

#include "kte/game/game_manager.h"
#include "kte/graphics/render_manager.h"
#include "kte/graphics/framebuffer.h"
#include "kte/graphics/window.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
StartFrameCommand::StartFrameCommand() = default;

////////////////////////////////////////////////////////////////
StartFrameCommand::~StartFrameCommand() = default;

////////////////////////////////////////////////////////////////
void StartFrameCommand::exec() const
{
  const auto& manager = GameManager::Instance();
  const auto& render_manager = manager.render_manager_;

  manager.window_.startFrame();

  if (render_manager.use_vr_)
  {
#ifdef OCULUS
    auto& oculus_data = const_cast<OculusHMD&>(render_manager.oculus_hmd_);
    oculus_data.startFrame(render_manager.frame_);
#endif
  }
}

} // end namespace graphics
} // end namespace kte

