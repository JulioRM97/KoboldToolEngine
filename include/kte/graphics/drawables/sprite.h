#pragma once

#include "kte/graphics/drawable.h"

#include <memory>

#include <glm/vec2.hpp>

namespace kte
{
namespace graphics
{

class Material;
class Texture;
class SpriteRendererProgram;

class Sprite : public Drawable
{
 public:
  Sprite();

  virtual ~Sprite();

  void load() override;

  void draw() const override;

  void unload() override;

  Program& program() const override;

  std::shared_ptr<Texture> texture_;

  glm::vec2 center_{0.5f, 0.5f};

  float texture_scale_ = 0.01f;

  struct GPUData;

  std::shared_ptr<GPUData> data_;

 private:
  std::shared_ptr<SpriteRendererProgram> program_;
};

} // end namespace graphics
} // end namespace kte

