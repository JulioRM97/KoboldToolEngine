#pragma once

#include "kte/graphics/drawable.h"

#include <string>
#include <memory>

namespace kte
{
namespace graphics
{

class Program;

class Skybox : public Drawable
{
 public:
  Skybox();

  virtual ~Skybox();

  void loadFiles(std::string nx,
                 std::string ny,
                 std::string nz,
                 std::string px,
                 std::string py,
                 std::string pz);

  void load() override;

  void draw() const override;

  void unload() override;

  Program& program() const override;

  struct GPUData;

  std::shared_ptr<GPUData> data_;

 private:
  std::shared_ptr<Program> program_;

};

} // end namespace graphics
} // end namespace kte

