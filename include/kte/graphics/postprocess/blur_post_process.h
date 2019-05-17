#pragma once

#include "kte/graphics/post_process.h"

namespace kte
{
namespace graphics
{

class BlurPostProcess : public PostProcess
{
 public:
  BlurPostProcess();

  ~BlurPostProcess() override;

 protected:
  void renderPass(std::shared_ptr<Framebuffer> output,
                  const LightComponent* light,
                  const PostProcessPass& pass,
                  DisplayList& list) const override;

};

} // end namespace graphics
} // end namespace kte

