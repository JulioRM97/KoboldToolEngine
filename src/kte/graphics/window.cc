#include "kte/graphics/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <kte/graphics/framebuffer.h>

#include "kte/game/game_manager.h"
#include "kte/game/components/camera_component.h"
#include "kte/graphics/imgui/imgui_impl_glfw_gl3.h"
#include "kte/input/input_manager.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
struct WindowData
{
  GLFWwindow* window = nullptr;

  glm::ivec2 size;

  glm::dvec2 mouse_pos;

  glm::dvec2 mouse_axis;
};

////////////////////////////////////////////////////////////////
struct Window::Data : public WindowData {};

////////////////////////////////////////////////////////////////
Window::Window()
    : data_(new Data())
{
}

////////////////////////////////////////////////////////////////
Window::~Window() = default;

////////////////////////////////////////////////////////////////
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);

  auto& manager = kte::GameManager::Instance();
  auto size = glm::ivec2(width, height);

  static_cast<WindowData*>(glfwGetWindowUserPointer(window))->size = size;

  if (manager.current_camera_)
  {
    manager.current_camera_->set_screen_size(size);
  }

  // FIXME Framebuffer::ScreenFramebuffer()->set_size(size);
}

////////////////////////////////////////////////////////////////
bool Window::init(std::string title, uint32_t width, uint32_t height)
{
  if (!glfwInit())
  {
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  data_->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  if (!data_->window)
  {
    glfwTerminate();
    return false;
  }

  glfwSetWindowUserPointer(data_->window, data_.get());
  glfwSetFramebufferSizeCallback(data_->window, framebuffer_size_callback);
  glfwGetWindowSize(data_->window, &data_->size.x, &data_->size.y);

  ImGui_ImplGlfwGL3_Init(data_->window, true);

  glfwMakeContextCurrent(data_->window);

  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  printf("OpenGL Version %d.%d loaded: %s\n",
         GLVersion.major, GLVersion.minor, glGetString(GL_VERSION));

  glfwSwapInterval(1);

  // TODO use callback, see http://www.glfw.org/docs/latest/input_guide.html#input_key
  // glfwSetKeyCallback(data_->window, ___);
  glfwSetInputMode(data_->window, GLFW_STICKY_KEYS, 0);

  framebuffer_size_callback(data_->window, data_->size.x, data_->size.y);

  ImGui_ImplGlfwGL3_Init(data_->window, true);

  return true;
}

////////////////////////////////////////////////////////////////
bool Window::processEvents()
{
  if (glfwWindowShouldClose(data_->window))
  {
    return false;
  }

  glfwPollEvents();

  {
    glm::dvec2 new_mouse_pos{};
    glfwGetCursorPos(data_->window, &new_mouse_pos.x, &new_mouse_pos.y);
    data_->mouse_axis = new_mouse_pos - data_->mouse_pos;
    data_->mouse_pos = new_mouse_pos;
  }

  // Global keys
  if (glfwGetKey(data_->window, GLFW_KEY_F3) == GLFW_PRESS)
  {
    enable_imgui_ = !enable_imgui_;
  }

  if (glfwGetKey(data_->window, GLFW_KEY_F4) == GLFW_PRESS)
  {
    GameManager::Instance().current_camera_->set_render_mode(RenderMode::k_Forward);
  }

  if (glfwGetKey(data_->window, GLFW_KEY_F5) == GLFW_PRESS)
  {
    GameManager::Instance().current_camera_->set_render_mode(RenderMode::k_Deferred);
  }

  if (glfwGetKey(data_->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////
bool Window::keyPressed(Key key) const
{
  return glfwGetKey(data_->window, key) == GLFW_PRESS;
}

////////////////////////////////////////////////////////////////
bool Window::mouseButtonPressed(uint8_t button) const
{
  return glfwGetMouseButton(data_->window, button) == GLFW_PRESS;
}

////////////////////////////////////////////////////////////////
bool Window::keyReleased(Key key) const
{
  return glfwGetKey(data_->window, key) == GLFW_RELEASE;
}

////////////////////////////////////////////////////////////////
bool Window::mouseButtonReleased(uint8_t button) const
{
  return glfwGetMouseButton(data_->window, button) == GLFW_RELEASE;
}

////////////////////////////////////////////////////////////////
float Window::mouseAxisValue(int axis) const
{
  return (float) data_->mouse_axis[axis];
}

////////////////////////////////////////////////////////////////
void Window::captureMouse(bool capture) const
{
  auto new_mode = capture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
  if (glfwGetInputMode(data_->window, GLFW_CURSOR) != new_mode)
  {
    glfwSetInputMode(data_->window, GLFW_CURSOR, new_mode);
  }
}

////////////////////////////////////////////////////////////////
void Window::startFrame() const
{
  if (enable_imgui_)
  {
    ImGui_ImplGlfwGL3_NewFrame();
  }
}

////////////////////////////////////////////////////////////////
void Window::swap() const
{
  if (enable_imgui_)
  {
    ImGui::Render();
  }

  glfwSwapBuffers(data_->window);
}

////////////////////////////////////////////////////////////////
void Window::finish()
{
  glfwTerminate();
}

////////////////////////////////////////////////////////////////
glm::ivec2 Window::size() const
{
  return data_->size;
}

////////////////////////////////////////////////////////////////
void Window::set_framerate_limit(bool limit)
{
  glfwSwapInterval(limit ? 1 : 0);
}

} // end namespace graphics
} // end namespace kte
