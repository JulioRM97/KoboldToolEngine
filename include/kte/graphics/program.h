#pragma once

#include <memory>

#include <glm/detail/type_mat.hpp>

#include "kte/types.h"

namespace kte
{

class LightComponent;

namespace graphics
{

class Drawable;

class Program
{
 public:
  struct GPUData;

  virtual void load() = 0;

  virtual void draw(const Drawable& drawable,
                    const LightComponent* light,
                    const glm::mat4& model,
                    const glm::mat4& view,
                    const glm::mat4& projection,
                    RenderMode mode) const = 0;

  bool supports(RenderMode mode) const;

  std::shared_ptr<GPUData> data_;

 protected:
  Program();

  explicit Program(GPUData* data);

  virtual ~Program();
};

} // end namespace graphics
} // end namespace kte

