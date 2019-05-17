#pragma once

#include "kte/graphics/post_process.h"

namespace kte
{
namespace graphics
{

class RenderDepthPostProcess : public PostProcess
{
 public:
  RenderDepthPostProcess();

  ~RenderDepthPostProcess() override;
};

}
}



