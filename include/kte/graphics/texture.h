#pragma once

#include <memory>
#include <string>

#include <glm/vec2.hpp>

namespace kte
{
namespace graphics
{

class Texture
{
 public:
  static std::shared_ptr<Texture> createFromFile(std::string file_name);

  Texture() = default;

  virtual ~Texture() = default;

  virtual void load() = 0;

  virtual void generateMipmap() = 0;

  virtual void unload() = 0;

  // TODO maybe we don't need to expose this?
  virtual unsigned int internal_id() const = 0;

  virtual glm::uvec2 size() const = 0;
};

} // end namespace graphics
} // end namespace kte

