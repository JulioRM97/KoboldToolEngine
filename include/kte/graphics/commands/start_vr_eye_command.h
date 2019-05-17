#pragma once

#include "kte/graphics/display/display_list_command.h"

namespace kte
{

class Window;

namespace graphics
{

class StartVrEyeCommand : public DisplayListCommand
{
 public:
  StartVrEyeCommand(int eye, const CameraParameters& camera_parameters);

  ~StartVrEyeCommand() override;

  void exec() const override;

 private:
  int eye_;

  const CameraParameters& camera_parameters_;
};

} // end namespace graphics
} // end namespace kte

