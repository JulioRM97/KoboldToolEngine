#include "kte/graphics/commands/start_vr_eye_command.h"
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
StartVrEyeCommand::StartVrEyeCommand(int eye, const CameraParameters& camera_parameters)
    : eye_(eye),
      camera_parameters_(camera_parameters)
{
}

////////////////////////////////////////////////////////////////
StartVrEyeCommand::~StartVrEyeCommand() = default;

////////////////////////////////////////////////////////////////
void StartVrEyeCommand::exec() const
{
#ifdef OCULUS
    const auto& manager = GameManager::Instance();
    const auto& render_manager = manager.render_manager_;
    auto& oculus_data = const_cast<OculusHMD&>(render_manager.oculus_hmd_);
    oculus_data.prepareEye(eye_, camera_parameters_);
#endif
}

} // end namespace graphics
} // end namespace kte

