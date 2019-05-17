#ifdef OCULUS

#pragma once

#include "kte/game/components/transform_component.h"
#include "kte/types.h"
#include "kte/graphics/vr/vr_common.h"

namespace kte
{
namespace graphics
{

class OculusHMDEyeCameraParameters : public CameraParameters
{
 public:
  std::shared_ptr<Framebuffer> framebuffer() const;

  const kte::TransformComponent& transform() const override;

  glm::mat4 projection() const override { return projection_; }

  glm::mat4 view() const override { return view_; }

  rect2i viewport() const override;

  glm::mat4 projection_{1};

  glm::mat4 view_{1};

  struct OculusTextureBuffer* eye_render_texture = nullptr;
};

class OculusHMD
{
 public:
  bool init();

  void updateSessionStatus();

  bool startFrame(long long int frame);

  void prepareEye(int eye, const CameraParameters& camera_parameters);

  void submitEye(int eye, const CameraParameters& camera_parameters);

  void renderMirror(class DisplayList& list);

  bool submitFrame(long long int frame);

  void shutdown();

  ovrSession session_ = nullptr;

  ovrHmdDesc hmd_desc_ = {};

  ovrSessionStatus session_status_ = {};

  ovrPosef hmd_to_eye_poses[2];

  ovrTimewarpProjectionDesc pos_timewarp_projection_desc_ = {};

  double sensor_sample_time_;

  ovrMirrorTexture mirror_texture_ = nullptr;

  std::shared_ptr<Framebuffer> mirror_framebuffer_;

  OculusHMDEyeCameraParameters eye_camera_parameters_[2];

  ovrPosef eye_poses[2];
};

} // end namespace graphics
} // end namespace kte

#endif // OCULUS

