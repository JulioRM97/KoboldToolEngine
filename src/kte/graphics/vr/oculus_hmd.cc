#ifdef OCULUS

#include "kte/graphics/vr/oculus_hmd.h"
#include "kte/graphics/gl_internals.h"

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "kte/game/game_manager.h"
#include "kte/game/components/camera_component.h"
#include "kte/graphics/window.h"
#include "kte/graphics/framebuffer.h"
#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/commands/copy_framebuffers_command.h"
#include "kte/graphics/display/display_list.h"

using namespace OVR;

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
struct OculusTextureBuffer
{
  ovrSession session;

  ovrTextureSwapChain color_texture_chain = nullptr;

  ovrTextureSwapChain depth_texture_chain = nullptr;

  std::shared_ptr<kte::graphics::Framebuffer> framebuffer;

  Sizei size;

  std::vector<std::shared_ptr<TextureAttachment>> color_textures;

  std::vector<std::shared_ptr<TextureAttachment>> depth_textures;

  OculusTextureBuffer(ovrSession session, ovrSizei size, int sampleCount)
      : session(session),
        framebuffer(std::make_shared<kte::graphics::Framebuffer>()),
        size(size)
  {
    // The code doesn't currently handle MSAA textures.
    assert(sampleCount <= 1);

    // This texture isn't necessarily going to be a rendertarget, but it usually is.
    assert(session); // No HMD? A little odd.

    ovrTextureSwapChainDesc desc = {};
    desc.Type = ovrTexture_2D;
    desc.ArraySize = 1;
    desc.Width = size.w;
    desc.Height = size.h;
    desc.MipLevels = 1;
    desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
    desc.SampleCount = sampleCount;
    desc.StaticImage = ovrFalse;

    framebuffer->data_->srgb = true;

    // Create color textures
    {
      ovrResult result = ovr_CreateTextureSwapChainGL(session, &desc, &color_texture_chain);
      if (OVR_SUCCESS(result))
      {
        int length = 0;
        ovr_GetTextureSwapChainLength(session, color_texture_chain, &length);
        color_textures.resize(length);
        for (int i = 0; i < length; ++i)
        {
          GLuint texture_id;
          ovr_GetTextureSwapChainBufferGL(session, color_texture_chain, i, &texture_id);
          color_textures[i] = std::make_shared<TextureAttachment>(texture_id, glm::make_vec2(&size.w));
          color_textures[i]->set_generate_mipmap(true);
        }
      }
    }

    desc.Format = OVR_FORMAT_D32_FLOAT;

    // Create depth textures
    {
      ovrResult result = ovr_CreateTextureSwapChainGL(session, &desc, &depth_texture_chain);
      if (OVR_SUCCESS(result))
      {
        int length = 0;
        ovr_GetTextureSwapChainLength(session, depth_texture_chain, &length);
        depth_textures.resize(length);
        for (int i = 0; i < length; ++i)
        {
          GLuint texture_id;
          ovr_GetTextureSwapChainBufferGL(session, depth_texture_chain, i, &texture_id);
          depth_textures[i] = std::make_shared<TextureAttachment>(texture_id, glm::make_vec2(&size.w));
        }
      }
    }
  }

  ~OculusTextureBuffer()
  {
    if (color_texture_chain)
    {
      ovr_DestroyTextureSwapChain(session, color_texture_chain);
      color_texture_chain = nullptr;
    }

    if (depth_texture_chain)
    {
      ovr_DestroyTextureSwapChain(session, depth_texture_chain);
      depth_texture_chain = nullptr;
    }
  }

  void setupFramebuffer()
  {
    int idx;

    ovr_GetTextureSwapChainCurrentIndex(session, color_texture_chain, &idx);
    framebuffer->bindColorTextureAttachment(color_textures[idx]);

    ovr_GetTextureSwapChainCurrentIndex(session, depth_texture_chain, &idx);
    framebuffer->bindDepthTextureAttachment(depth_textures[idx]);
  }

  void resetFramebuffer()
  {
    //framebuffer->unbindColorTextureAttachment(0);
    //framebuffer->unbindDepthTextureAttachment();
  }

  void commit()
  {
    ovr_CommitTextureSwapChain(session, color_texture_chain);
    ovr_CommitTextureSwapChain(session, depth_texture_chain);
  }
};

////////////////////////////////////////////////////////////////
rect2i OculusHMDEyeCameraParameters::viewport() const
{
  return rect2i{glm::vec2(0, 0),
                glm::make_vec2(&eye_render_texture->size.w)};
}

////////////////////////////////////////////////////////////////
const kte::TransformComponent& OculusHMDEyeCameraParameters::transform() const
{
  return GameManager::Instance().current_camera_->transform();
}

////////////////////////////////////////////////////////////////
bool OculusHMD::init()
{
  printf("Initializing Oculus VR\n");

  ovrInitParams init_params = { ovrInit_RequestVersion | ovrInit_FocusAware, OVR_MINOR_VERSION, NULL, 0, 0 };
  assert_vr(ovr_Initialize(&init_params));

  ovrResult result;
  ovrGraphicsLuid luid;
  result = ovr_Create(&session_, &luid);
  if (check_oculus_error(result, "No device could be found"))
  {
    ovr_Shutdown();
    return false;
  }

  hmd_desc_ = ovr_GetHmdDesc(session_);

  printf("Product:         %s\n", hmd_desc_.ProductName);
  printf("Manufacturer:    %s\n", hmd_desc_.Manufacturer);
  printf("VendorId:        %hi\n", hmd_desc_.VendorId);
  printf("ProductId:       %hi\n", hmd_desc_.ProductId);
  printf("SerialNumber:    %s\n", hmd_desc_.SerialNumber);
  printf("FirmwareVersion: %hi.%hi\n",
         hmd_desc_.FirmwareMajor,
         hmd_desc_.FirmwareMinor);

  // FIXME: make this the screen size????
  ovrSizei windowSize = { hmd_desc_.Resolution.w / 2, hmd_desc_.Resolution.h / 2 };

  // Make eye render buffers
  for (int eye = 0; eye < 2; ++eye)
  {
    ovrSizei idealTextureSize = ovr_GetFovTextureSize(session_, ovrEyeType(eye), hmd_desc_.DefaultEyeFov[eye], 1);
    eye_camera_parameters_[eye].eye_render_texture = new OculusTextureBuffer(session_, idealTextureSize, 1);

    if (!eye_camera_parameters_[eye].eye_render_texture->color_texture_chain ||
        !eye_camera_parameters_[eye].eye_render_texture->depth_texture_chain)
    {
      fprintf(stderr, "Could not create eye render textures");
      return false;
    }
  }

  ovrMirrorTextureDesc desc = {};
  desc.Width = windowSize.w;
  desc.Height = windowSize.h;
  desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

  // Create mirror texture and an FBO used to copy mirror texture to back buffer
  assert_vr(ovr_CreateMirrorTextureWithOptionsGL(session_, &desc, &mirror_texture_));

  // Configure the mirror read buffer
  GLuint texId;
  ovr_GetMirrorTextureBufferGL(session_, mirror_texture_, &texId);
  auto texture = std::make_shared<TextureAttachment>(texId, glm::make_vec2(&windowSize.w));

  mirror_framebuffer_ = std::make_shared<kte::graphics::Framebuffer>();
  mirror_framebuffer_->bindColorTextureAttachment(texture);
  mirror_framebuffer_->addDepthTextureAttachment();

  GameManager::Instance().window_.set_framerate_limit(false);

  ovr_SetTrackingOriginType(session_, ovrTrackingOrigin_FloorLevel);

  printf("Oculus VR initialized correctly\n");

  return true;
}

////////////////////////////////////////////////////////////////
std::shared_ptr<Framebuffer> OculusHMDEyeCameraParameters::framebuffer() const
{
  return eye_render_texture->framebuffer;
}

const bool gUseNewOculusAPI = false;

////////////////////////////////////////////////////////////////
void OculusHMD::updateSessionStatus()
{
  ovr_GetSessionStatus(session_, &session_status_);
}

////////////////////////////////////////////////////////////////
bool OculusHMD::startFrame(long long int frame)
{
  if (session_status_.ShouldQuit)
  {
    // Stop running as soon as possible
    GameManager::Instance().running_ = false;
  }

  if (session_status_.ShouldRecenter)
  {
    ovr_RecenterTrackingOrigin(session_);
  }

  if (gUseNewOculusAPI)
  {
    fprintf(stderr, "################### ovr_WaitToBeginFrame(session_, %lli)\n", frame);
    assert_vr(ovr_WaitToBeginFrame(session_, frame));
    double frame_timing = ovr_GetPredictedDisplayTime(session_, frame);

    // Get the corresponding predicted pose state.
    ovrTrackingState state = ovr_GetTrackingState(session_, frame_timing, ovrTrue);
    ovr_CalcEyePoses(state.HeadPose.ThePose, hmd_to_eye_poses, eye_poses);

    fprintf(stderr, "################### ovr_BeginFrame(session_, %lli)\n", frame);
    assert_vr(ovr_BeginFrame(session_, frame));
  }
  else
  {
    // Call ovr_GetRenderDesc each frame to get the ovrEyeRenderDesc, as the returned values (e.g. hmd_to_eye_poses) may change at runtime.
    ovrEyeRenderDesc eye_render_desc[2];
    eye_render_desc[0] = ovr_GetRenderDesc(session_, ovrEye_Left, hmd_desc_.DefaultEyeFov[0]);
    eye_render_desc[1] = ovr_GetRenderDesc(session_, ovrEye_Right, hmd_desc_.DefaultEyeFov[1]);

    // Get eye poses, feeding in correct IPD offset
    ovrPosef hmd_to_eye_poses[2] = { eye_render_desc[0].HmdToEyePose,
                                     eye_render_desc[1].HmdToEyePose};

    ovr_GetEyePoses(session_, frame, ovrTrue, hmd_to_eye_poses, eye_poses,
                    &sensor_sample_time_);
  }

  return true;
}

////////////////////////////////////////////////////////////////
void OculusHMD::prepareEye(int eye, const CameraParameters& camera_parameters)
{
  // TODO use camera parameters
  // Get view and projection matrices for the camera
  glm::vec3 originPos = camera_parameters.transform().world_translation();
  glm::quat originRot = camera_parameters.transform().local_rotation();

  // Get view and projection matrices
  Matrix4f rollPitchYaw = Matrix4f::RotationY(0);
  Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(eye_poses[eye].Orientation);
  Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
  Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
  Vector3f shiftedEyePos = rollPitchYaw.Transform(eye_poses[eye].Position);

  Matrix4f view = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);

  // Get view and projection matrices
  auto m = ovrMatrix4f_Projection(hmd_desc_.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_None);
  eye_camera_parameters_[eye].view_ = glm::transpose(glm::make_mat4(&view.M[0][0]));
  eye_camera_parameters_[eye].projection_ = glm::transpose(glm::make_mat4(&m.M[0][0]));

  pos_timewarp_projection_desc_ = ovrTimewarpProjectionDesc_FromProjection(m, ovrProjection_None);

  // Setup eye framebuffer
  eye_camera_parameters_[eye].eye_render_texture->setupFramebuffer();
}

////////////////////////////////////////////////////////////////
void OculusHMD::submitEye(int eye, const CameraParameters& camera_parameters)
{
  eye_camera_parameters_[eye].eye_render_texture->resetFramebuffer();
  eye_camera_parameters_[eye].eye_render_texture->commit();
}

////////////////////////////////////////////////////////////////
void OculusHMD::renderMirror(DisplayList& list)
{
  // TODO
  list.add<CopyFramebuffersCommand>(mirror_framebuffer_, Framebuffer::ScreenFramebuffer());
}

////////////////////////////////////////////////////////////////
bool OculusHMD::submitFrame(long long int frame)
{
  ovrLayerEyeFovDepth ld = {};
  ld.Header.Type  = ovrLayerType_EyeFovDepth;
  ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;
  ld.ProjectionDesc = pos_timewarp_projection_desc_;

  for (int eye = 0; eye < 2; eye++)
  {
    ld.ColorTexture[eye] = eye_camera_parameters_[eye].eye_render_texture->color_texture_chain;
    ld.DepthTexture[eye] = eye_camera_parameters_[eye].eye_render_texture->depth_texture_chain;
    ld.Viewport[eye]     = Recti(eye_camera_parameters_[eye].eye_render_texture->size);
    ld.Fov[eye]          = hmd_desc_.DefaultEyeFov[eye];
    ld.RenderPose[eye]   = eye_poses[eye];
    ld.SensorSampleTime  = sensor_sample_time_;
  }

  ovrLayerHeader* layers = &ld.Header;
  if (gUseNewOculusAPI)
  {
    assert_vr(ovr_EndFrame(session_, frame, nullptr, &layers, 1));
  }
  else
  {
    assert_vr(ovr_SubmitFrame(session_, frame, nullptr, &layers, 1));
  }

  return true;
}

////////////////////////////////////////////////////////////////
void OculusHMD::shutdown()
{
  ovr_Destroy(session_);
  ovr_Shutdown();
  printf("Oculus VR shutdown correctly\n");
}

} // end namespace graphics
} // end namespace kte

#endif // OCULUS

