#include "kte/game/components/camera_component.h"

#include <glm/ext.hpp>

#include "kte/editor/draw_editor_command.h"
#include "kte/game/game_manager.h"
#include "kte/game/scene.h"
#include "kte/game/node.h"
#include "kte/graphics/post_process.h"
#include "kte/graphics/render_manager.h"
#include "kte/graphics/display/display_list_buffer.h"

using namespace kte::graphics;

namespace kte
{

////////////////////////////////////////////////////////////////
struct CameraComponent::Data
{
  std::vector<std::shared_ptr<PostProcess>> post_process_list;
  glm::mat4 transform{1};
  glm::mat4 view{1};
  RenderMode render_mode = RenderMode::k_Forward;
};

////////////////////////////////////////////////////////////////
CameraComponent::CameraComponent(Node& parent)
    : Component(parent),
      data_(new Data())
{
}

////////////////////////////////////////////////////////////////
CameraComponent::~CameraComponent() = default;

////////////////////////////////////////////////////////////////
void CameraComponent::set_perspective(float yfov,
                                      float aspect_ratio,
                                      float znear,
                                      float zfar)
{
  data_->transform = glm::perspective(yfov, aspect_ratio, znear, zfar);
}

////////////////////////////////////////////////////////////////
void CameraComponent::set_orthographic(float xmag,
                                       float ymag,
                                       float znear,
                                       float zfar)
{
  const float dz = znear - zfar;

  data_->transform = glm::mat4 {{1 / xmag, 0, 0, 0},
                                {0, 1 / ymag, 0, 0},
                                {0, 0, 2 / dz, (znear + zfar) / dz},
                                {0, 0, 0, 1}};
}

////////////////////////////////////////////////////////////////
void CameraComponent::update()
{
  data_->view = glm::inverse(owner_node_.transform()->world_transform());

  auto& render_manager = GameManager::Instance().render_manager_;
  render_manager.draw(*owner_node_.scene_, *this, data_->render_mode);
}

////////////////////////////////////////////////////////////////
void CameraComponent::addPostProcess(std::shared_ptr<PostProcess> post_process)
{
  data_->post_process_list.push_back(post_process);
}

////////////////////////////////////////////////////////////////
void CameraComponent::set_screen_size(glm::ivec2 screen_size)
{
  // TODO update view matrix

  for (const auto& post_process : data_->post_process_list)
  {
    post_process->set_screen_size(screen_size);
  }
}

////////////////////////////////////////////////////////////////
void CameraComponent::set_render_mode(kte::graphics::RenderMode render_mode)
{
  data_->render_mode = render_mode;
}

////////////////////////////////////////////////////////////////
const TransformComponent& CameraComponent::transform() const
{
  return *owner_node_.transform();
}

////////////////////////////////////////////////////////////////
glm::mat4 CameraComponent::projection() const
{
  return data_->transform;
}

////////////////////////////////////////////////////////////////
glm::mat4 CameraComponent::view() const
{
  return data_->view;
}

////////////////////////////////////////////////////////////////
rect2i CameraComponent::viewport() const
{
  return rect2i();
}

////////////////////////////////////////////////////////////////
const std::vector<std::shared_ptr<kte::graphics::PostProcess>> CameraComponent::post_process_list() const
{
  return data_->post_process_list;
}

} // end namespace kte

