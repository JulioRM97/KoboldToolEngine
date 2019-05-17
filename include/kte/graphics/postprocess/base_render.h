#pragma once

#include "kte/graphics/post_process.h"

namespace kte
{
namespace graphics
{

class BaseRender : public PostProcess
{
 public:
  BaseRender();

  ~BaseRender() override;
};

} // end namespace graphics
} // end namespace kte

