#pragma once

#include "kte/graphics/program.h"

#include <glm/vec2.hpp>

namespace kte
{
namespace graphics
{

// Forward declarations
class Drawable;
class Sprite;

class SpriteRendererProgram : public Program
{
 public:
  SpriteRendererProgram();

  ~SpriteRendererProgram() override;

  void load() override;

  void draw(const Drawable& drawable,
            const LightComponent* light,
            const glm::mat4& model,
            const glm::mat4& view,
            const glm::mat4& projection,
            RenderMode mode) const override;

  void applySpriteParams(const Sprite& sprite) const;
};

} // end namespace graphics
} // end namespace kte



