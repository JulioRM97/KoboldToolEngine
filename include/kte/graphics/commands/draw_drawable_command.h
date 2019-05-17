#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <memory>

#include <glm/mat4x4.hpp>

namespace kte
{

// Forward declarations
class CameraComponent;
class LightComponent;

namespace graphics
{

// Forward declarations
class Drawable;

class DrawDrawableCommand : public DisplayListCommand
{
 public:
  DrawDrawableCommand(std::shared_ptr<Drawable> drawable,
                      glm::mat4 model,
                      const CameraParameters& camera_parameters,
                      const LightComponent* light,
                      RenderMode mode);

  ~DrawDrawableCommand() override;

  void exec() const override;

 private:
  struct Data;

  std::unique_ptr<Data> data_;
};

} // end namespace graphics
} // end namespace kte

