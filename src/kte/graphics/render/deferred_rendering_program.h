#pragma once

#include "kte/graphics/full_screen_program.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class Framebuffer;

class DeferredRenderingProgram : public FullScreenProgram
{
 public:
  DeferredRenderingProgram();

  ~DeferredRenderingProgram() override;

  void load() override;
  std::shared_ptr<Framebuffer> g_buffer_;


  std::vector<glm::vec3> ssao_noise_;
};

} // end namespace graphics
} // end namespace kte

