#pragma once

#include "kte/graphics/drawable.h"

#include <memory>

namespace kte
{
namespace graphics
{

class Material;

class Primitive : public Drawable
{
 public:
  Primitive();

  virtual ~Primitive();

  void load() override;

  void draw() const override;

  void unload() override;

  Program& program() const override;

  std::shared_ptr<Material> material_;

  struct GPUData;

  std::shared_ptr<GPUData> data_;
};

} // end namespace graphics
} // end namespace kte

