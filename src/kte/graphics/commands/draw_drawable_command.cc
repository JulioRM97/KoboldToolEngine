#include "kte/graphics/commands/draw_drawable_command.h"

#include "kte/graphics/drawable.h"
#include "kte/graphics/program.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
struct DrawDrawableCommand::Data
{
  std::shared_ptr<Drawable> drawable;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  const LightComponent* light;
  RenderMode mode;
};

////////////////////////////////////////////////////////////////
DrawDrawableCommand::DrawDrawableCommand(const std::shared_ptr<Drawable> drawable,
                                         const glm::mat4 model,
                                         const CameraParameters& camera_parameters,
                                         const LightComponent* light,
                                         RenderMode mode)
    : data_(new Data())
{
  data_->drawable = drawable;
  data_->model = model;
  data_->view = camera_parameters.view();
  data_->projection = camera_parameters.projection();
  data_->light = light;
  data_->mode = mode;
}

////////////////////////////////////////////////////////////////
DrawDrawableCommand::~DrawDrawableCommand() = default;

////////////////////////////////////////////////////////////////
void DrawDrawableCommand::exec() const
{
  data_->drawable->load();

  if (!data_->drawable->program().supports(data_->mode))
  {
    return;
  }

  data_->drawable->program().draw(*data_->drawable,
                                  data_->light,
                                  data_->model,
                                  data_->view,
                                  data_->projection,
                                  data_->mode);
}

} // end namespace graphics
} // end namespace kte

