#pragma once

#include "kte/game/component.h"

#include <memory>
#include <utility>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include "kte/types.h"

namespace kte
{

namespace graphics
{
class PostProcess;
}

class CameraComponent : public Component, public CameraParameters
{
 public:
  explicit CameraComponent(Node& parent);

  ~CameraComponent() override;

  void set_perspective(float yfov,
                       float aspect_ratio,
                       float znear,
                       float zfar);

  void set_orthographic(float xmag,
                        float ymag,
                        float znear,
                        float zfar);

  void update() override;

  template<typename T>
  inline void addPostProcess(T*&& post_process)
  {
    addPostProcess(std::shared_ptr<T>(post_process));
  }

  /// Create and add a post process from a template
  template<typename T, typename... Args>
  inline void addPostProcess(Args&& ... args)
  {
    addPostProcess(std::make_shared<T>(std::forward<Args>(args)...));
  }

  /// Add a post process
  void addPostProcess(std::shared_ptr<kte::graphics::PostProcess> post_process);

  void set_screen_size(glm::ivec2 screen_size);

  void set_render_mode(kte::graphics::RenderMode render_mode);

  const TransformComponent& transform() const override;

  glm::mat4 projection() const override;

  glm::mat4 view() const override;

  rect2i viewport() const override;

  const std::vector<std::shared_ptr<kte::graphics::PostProcess>> post_process_list() const;

 private:
  struct Data;

  std::unique_ptr<Data> data_;
};

} // end namespace kte

