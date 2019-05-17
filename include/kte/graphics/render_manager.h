#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "kte/types.h"
#include "kte/game/updatable.h"
#include "kte/graphics/vr/oculus_hmd.h"

// Forward declarations
namespace kte
{

class Node;
class Scene;
class CameraComponent;
class LightComponent;

namespace graphics
{

class DisplayList;
class Framebuffer;
class TextureAttachment;

} // end namespace graphics
} // end namespace kte

namespace kte
{
namespace graphics
{

class PostProcess;
class DeferredRenderingProgram;

class RenderManager : public DefaultUpdatable
{
 public:
  RenderManager();

  ~RenderManager();

  void init();

  void draw(const Scene& scene,
            const CameraComponent& camera,
            RenderMode mode);

  long long int frame_ = 1;

  bool use_vr_ = false;

#ifdef OCULUS
  OculusHMD oculus_hmd_;
#endif

 private:
  void performRender(const Scene& scene,
                     const std::vector<std::shared_ptr<kte::graphics::PostProcess>>& post_process_list,
                     const CameraParameters& camera_parameters,
                     const std::shared_ptr<Framebuffer>& final_output,
                     DisplayList& list,
                     const RenderMode& mode) const;

  void forwardRender(const Scene& scene,
                     const CameraParameters& camera_parameters,
                     const std::shared_ptr<Framebuffer>& output,
                     DisplayList& list) const;

  void deferredRender(const Scene& scene,
                      const CameraParameters& camera_parameters,
                      const std::shared_ptr<Framebuffer>& output,
                      DisplayList& list) const;

  void renderShadowMap(const Scene& scene,
                       const LightComponent& light,
                       DisplayList& list) const;

  void draw(const Scene& scene,
            const CameraParameters& camera_parameters,
            const LightComponent* light,
            DisplayList& list,
            RenderMode mode) const;

  void draw(const Node& node,
            const CameraParameters& camera_parameters,
            const LightComponent* light,
            DisplayList& list,
            RenderMode mode) const;

  std::shared_ptr<DeferredRenderingProgram> deferred_;
};

} // end namespace graphics
} // end namespace kte

