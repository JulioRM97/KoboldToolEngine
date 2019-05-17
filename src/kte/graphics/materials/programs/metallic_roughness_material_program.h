#pragma once

#include "kte/graphics/program.h"

namespace kte
{
namespace graphics
{

class MetallicRoughnessMaterialProgram : public Program
{
 public:
  MetallicRoughnessMaterialProgram();

  ~MetallicRoughnessMaterialProgram() override;

  void load() override;

  void draw(const Drawable& drawable,
            const LightComponent* light,
            const glm::mat4& model,
            const glm::mat4& view,
            const glm::mat4& projection,
            RenderMode mode) const override;
};

} // end namespace graphics
} // end namespace kte



