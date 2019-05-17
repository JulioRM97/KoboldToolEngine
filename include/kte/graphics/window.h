#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>

#include "kte/types.h"

namespace kte
{
namespace graphics
{

class Window
{
 public:
  Window();

  ~Window();

  bool init(std::string title, uint32_t width, uint32_t height);

  bool processEvents();

  bool keyPressed(Key key) const;

  bool mouseButtonPressed(uint8_t button) const;

  bool keyReleased(Key key) const;

  bool mouseButtonReleased(uint8_t button) const;

  float mouseAxisValue(int axis) const;

  void captureMouse(bool capture = true) const;

  void startFrame() const;

  void swap() const;

  void finish();

  glm::ivec2 size() const;

  void set_framerate_limit(bool limit);

  bool enable_imgui_ = false;

 private:
  struct Data;

  std::unique_ptr<Data> data_;
};

} // end namespace graphics
} // end namespace kte

