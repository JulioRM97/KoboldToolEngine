#include "kte/game/components/vr_pose_tracker_component.h"

#include <glm/ext.hpp>

#ifdef OCULUS
#include <OVR_CAPI_GL.h>
#endif

#include "kte/game/game_manager.h"
#include "kte/game/node.h"
#include "kte/game/components/transform_component.h"
#include "kte/graphics/render_manager.h"

namespace kte
{

////////////////////////////////////////////////////////////////
VrPoseTrackerComponent::VrPoseTrackerComponent(Node& parent)
    : Component(parent)
{
}

////////////////////////////////////////////////////////////////
VrPoseTrackerComponent::~VrPoseTrackerComponent() = default;

////////////////////////////////////////////////////////////////
void VrPoseTrackerComponent::fixedUpdate()
{
#ifdef OCULUS
  auto& render_manager = GameManager::Instance().render_manager_;

  // Query the HMD for ts current tracking state.
  auto time = ovr_GetPredictedDisplayTime(render_manager.oculus_hmd_.session_, 0);
  auto state = ovr_GetTrackingState(render_manager.oculus_hmd_.session_, time, ovrTrue);

  if (state.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
  {
    ovrPosef& pose = state.HeadPose.ThePose;
    owner_node_.transform()->set_local_translation(glm::make_vec3(&pose.Position.x));
    owner_node_.transform()->set_local_rotation(glm::make_quat(&pose.Orientation.x));
  }
#endif
}

} // end namespace kte

