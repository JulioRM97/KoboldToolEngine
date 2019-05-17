#include "kte/graphics/render_manager.h"
#include "kte/graphics/vr/vr_common.h"

#include <algorithm>

#include "kte/editor/draw_editor_command.h"
#include "kte/game/node.h"
#include "kte/game/components/camera_component.h"
#include "kte/game/components/drawable_component.h"
#include "kte/game/components/light_component.h"
#include "kte/game/game_manager.h"
#include "kte/graphics/texture_attachment.h"
#include "kte/graphics/window.h"
#include "kte/graphics/post_process.h"
#include "kte/graphics/commands/clear_framebuffer_command.h"
#include "kte/graphics/commands/enable_framebuffer_command.h"
#include "kte/graphics/commands/print_debug_information_command.h"
#include "kte/graphics/commands/render_full_screen_command.h"
#include "kte/graphics/commands/start_frame_command.h"
#include "kte/graphics/commands/swap_buffers_command.h"
#include "kte/graphics/commands/start_vr_eye_command.h"
#include "kte/graphics/commands/submit_vr_eye_command.h"
#include "kte/graphics/display/display_list_buffer.h"
#include "kte/graphics/render/deferred_rendering_program.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
RenderManager::RenderManager()
    :   deferred_(std::make_shared<DeferredRenderingProgram>())
{
}

////////////////////////////////////////////////////////////////
RenderManager::~RenderManager()
{
#ifdef OCULUS
  use_vr_ = false;
  oculus_hmd_.shutdown();
#endif
}

////////////////////////////////////////////////////////////////
void RenderManager::init()
{
#ifdef OCULUS
  //use_vr_ = oculus_hmd_.init();
#endif
}

////////////////////////////////////////////////////////////////
void RenderManager::draw(const Scene& scene,
                         const CameraComponent& camera,
                         RenderMode mode)
{
  auto& manager = GameManager::Instance();
  const auto& post_process_list = camera.post_process_list();
  auto list = manager.display_list_buffer_.createList();

  if (!use_vr_)
  {
    list->add<StartFrameCommand>();

    performRender(scene, post_process_list, camera,
                  Framebuffer::ScreenFramebuffer(), *list, mode);

    list->add<DrawEditorCommand>();
    list->add<SwapBuffersCommand>();
  }
  else
  {
#ifdef OCULUS
    oculus_hmd_.updateSessionStatus();
    if (oculus_hmd_.session_status_.IsVisible)
    {
      list->add<StartFrameCommand>();

      for (int eye = 0; eye < 2; eye++)
      {
        list->add<StartVrEyeCommand>(eye, camera);

        const auto& eye_parameters = oculus_hmd_.eye_camera_parameters_[eye];
        performRender(scene, post_process_list, eye_parameters,
                      eye_parameters.framebuffer(), *list, mode);

        list->add<SubmitVrEyeCommand>(eye, camera);
      }

      list->add<SwapBuffersCommand>();
    }
    else
    {
      // Not rendering anything, wait for a bit
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
#endif
  }

  list->add<PrintDebugInformationCommand>();

  manager.display_list_buffer_.submit(list);
}

////////////////////////////////////////////////////////////////
void RenderManager::performRender(const Scene& scene,
                                  const std::vector<std::shared_ptr<kte::graphics::PostProcess>>& post_process_list,
                                  const CameraParameters& camera_parameters,
                                  const std::shared_ptr<Framebuffer>& final_output,
                                  DisplayList& list,
                                  const RenderMode& mode) const
{
  auto output = post_process_list.empty() ?
                final_output :
                post_process_list.front()->input_framebuffer();

  switch (mode)
  {
    case RenderMode::k_Forward:
    {
      forwardRender(scene, camera_parameters, output, list);
      break;
    }

    case RenderMode::k_Deferred:
    {
      deferredRender(scene, camera_parameters, output, list);
      break;
    }

    default:
    {
      fprintf(stderr, "RenderManager::performRender should not be called directly with this mode\n");
      break;
    }
  }

  const auto count = post_process_list.size();
  for (size_t i = 0; i < count; i++)
  {
    output = i + 1 == count ?
             final_output :
             post_process_list[i + 1]->input_framebuffer();
    post_process_list[i]->render(output, nullptr, list);
  }
}

////////////////////////////////////////////////////////////////
void RenderManager::forwardRender(const Scene& scene,
                                  const CameraParameters& camera_parameters,
                                  const std::shared_ptr<Framebuffer>& output,
                                  DisplayList& list) const
{
  list.add<ClearFramebufferCommand>(output);
  list.add<EnableFramebufferCommand>(output, camera_parameters, false, ZPrePassMode::k_OnlyZ);
  draw(scene, camera_parameters, nullptr, list, RenderMode::k_ZPrePass);

  const auto& lights = scene.getSystem<LightComponent>()->memory_manager_;
  for (const auto& light : lights)
  {
    // Render shadow map
    renderShadowMap(scene, light, list);

    // Render scene
    list.add<EnableFramebufferCommand>(output, camera_parameters, true, ZPrePassMode::k_ZEqual);
    draw(scene, camera_parameters, &light, list, RenderMode::k_Forward);
  }
}

////////////////////////////////////////////////////////////////
void RenderManager::deferredRender(const Scene& scene,
                                   const CameraParameters& camera_parameters,
                                   const std::shared_ptr<Framebuffer>& output,
                                   DisplayList& list) const
{
  deferred_->g_buffer_->set_size(GameManager::Instance().window_.size());

  // Render scene to g buffer
  list.add<ClearFramebufferCommand>(deferred_->g_buffer_);
  list.add<EnableFramebufferCommand>(deferred_->g_buffer_, camera_parameters, false);
  draw(scene, camera_parameters, nullptr, list, RenderMode::k_Deferred);

  // Render g buffer to output
  list.add<ClearFramebufferCommand>(output);

  const auto& lights = scene.getSystem<LightComponent>()->memory_manager_;
  for (const auto& light : lights)
  {
    renderShadowMap(scene, light, list);

    // Render scene
    list.add<EnableFramebufferCommand>(deferred_->g_buffer_, camera_parameters, true);
    list.add<RenderFullScreenCommand>(deferred_, deferred_->g_buffer_, output, &light);
  }
}

////////////////////////////////////////////////////////////////
void RenderManager::renderShadowMap(const Scene& scene,
                                    const LightComponent& light,
                                    DisplayList& list) const
{
  if (!light.generate_shadow_map())
  {
    list.add<ClearFramebufferCommand>(light.shadow_frame_buffer_, glm::vec4(1.0f));
    return;
  }

  list.add<ClearFramebufferCommand>(light.shadow_frame_buffer_);
  list.add<EnableFramebufferCommand>(light.shadow_frame_buffer_, light, false, ZPrePassMode::k_OnlyZ);

  for (const auto& node : scene.node_manager_)
  {
    // TODO filter nodes depending on whether they cast shadows or not
    draw(node, light, &light, list, RenderMode::k_ShadowMap);
  }
}

////////////////////////////////////////////////////////////////
void RenderManager::draw(const Scene& scene,
                         const CameraParameters& camera_parameters,
                         const LightComponent* light,
                         DisplayList& list,
                         RenderMode mode) const
{
  for (const auto& node : scene.node_manager_)
  {
    draw(node, camera_parameters, light, list, mode);
  }
}

////////////////////////////////////////////////////////////////
void RenderManager::draw(const Node& node,
                         const CameraParameters& camera_parameters,
                         const LightComponent* light,
                         DisplayList& list,
                         RenderMode mode) const
{
  for (const auto& pair : node.components_)
  {
    const auto* drawable = dynamic_cast<DrawableComponent*>(pair.second);
    if (drawable)
    {
      drawable->draw(camera_parameters, light, list, mode);
    }
  }
}

} // end namespace graphics
} // end namespace kte

