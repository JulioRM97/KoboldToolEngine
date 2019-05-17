#pragma once

#include "kte/graphics/full_screen_program.h"

namespace kte
{
namespace graphics
{

class PostProcessProgram : public FullScreenProgram
{
 public:
  PostProcessProgram(std::string base_shader_name);

  ~PostProcessProgram() override;
};

} // end namespace graphics
} // end namespace kte

