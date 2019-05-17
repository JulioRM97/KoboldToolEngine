#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "kte/graphics/program.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class Framebuffer;
struct ProgramInfo;
class Texture;
class DataTexture;

class FullScreenProgram : public Program
{
 public:
  explicit FullScreenProgram(std::string base_shader_name);

  ~FullScreenProgram() override;

  void load() override;

  void draw(const Drawable& drawable,
            const LightComponent* light,
            const glm::mat4& model,
            const glm::mat4& view,
            const glm::mat4& projection,
            RenderMode mode) const override;

  void setupInputFramebuffer(const Framebuffer& framebuffer,
                             RenderMode mode);

  std::shared_ptr<DataTexture> noise_tex_;

  std::vector<glm::vec3> ssao_kernel_;

 private:
  void bindTexture(const Texture& texture,
                   const ProgramInfo& info,
                   uint8_t idx);
};

} // end namespace graphics
} // end namespace kte

