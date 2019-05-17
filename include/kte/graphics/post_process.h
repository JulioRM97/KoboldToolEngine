#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include "full_screen_program.h"

namespace kte
{

class LightComponent;

namespace graphics
{

// Forward declarations
class DisplayList;
class Framebuffer;
class Program;

struct PostProcessPass
{
  std::shared_ptr<FullScreenProgram> program;
  std::shared_ptr<Framebuffer> input_framebuffer;
};

class PostProcess
{
 public:
  PostProcess(std::initializer_list<std::string> names);

  explicit PostProcess(const std::string& name) : PostProcess({ name }) {};

  virtual ~PostProcess();

  virtual std::shared_ptr<Framebuffer> input_framebuffer() const;

  virtual void render(const std::shared_ptr<Framebuffer> output,
                      const LightComponent* light,
                      DisplayList& list) const;

  virtual void set_screen_size(glm::ivec2 screen_size);

 protected:
  virtual void renderPass(const std::shared_ptr<Framebuffer> output,
                          const LightComponent* light,
                          const PostProcessPass& pass,
                          DisplayList& list) const;

  std::vector<PostProcessPass> passes_;

};

} // end namespace graphics
} // end namespace kte

