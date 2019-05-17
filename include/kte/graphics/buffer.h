#pragma once

#include <memory>

namespace kte
{
namespace graphics
{

class Buffer
{
 public:
  Buffer();

  virtual ~Buffer();

  void load();

  void unload();

  struct GPUData;

  std::shared_ptr<GPUData> data_;
};

} // end namespace graphics
} // end namespace kte

