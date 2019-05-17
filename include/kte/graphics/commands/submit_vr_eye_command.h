#pragma once

#include "kte/graphics/display/display_list_command.h"

namespace kte
{

class Window;

namespace graphics
{

class SubmitVrEyeCommand : public DisplayListCommand
{
 public:
  SubmitVrEyeCommand(int eye, const CameraParameters& camera_parameters);

  ~SubmitVrEyeCommand() override;

  void exec() const override;

 private:
  int eye_;

  const CameraParameters& camera_parameters_;
};

} // end namespace graphics
} // end namespace kte

