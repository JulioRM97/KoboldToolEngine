#pragma once

#include "kte/graphics/post_process.h"

namespace kte
{
namespace graphics
{

class BloomPostProcess : public PostProcess
{
 public:
  BloomPostProcess();

  ~BloomPostProcess() override;

 protected:
  void renderPass(const std::shared_ptr<Framebuffer> output,
                  const LightComponent* light,
                  const PostProcessPass& pass,
                  DisplayList& list) const override;

};

} // end namespace graphics
} // end namespace kte

