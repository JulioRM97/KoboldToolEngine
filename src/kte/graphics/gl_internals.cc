#include "gl_internals.h"

#include <fstream>
#include <iostream>

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
void _checkErrors(std::string desc, const char* file, int line)
{
  GLenum err(glGetError());

  while (err != GL_NO_ERROR)
  {
    std::string error;

    switch (err)
    {
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;

      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;

      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;

      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;

      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;

      default:
        error = std::to_string((int) err);
    }

    std::cerr << "GL_" << error.c_str() << " - " << desc << " at "
              << file << ":" << line << std::endl;
    err = glGetError();
  }
}

////////////////////////////////////////////////////////////////
void _checkFrameBufferErrors(std::string desc, const char* file, int line)
{
  GLenum err(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER));

  while (err != GL_FRAMEBUFFER_COMPLETE)
  {
    std::string error;

    switch (err)
    {
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        error = "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        error = "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;

      case GL_FRAMEBUFFER_UNSUPPORTED:
        error = "FRAMEBUFFER_UNSUPPORTED";
        break;

      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;

      default:
        error = std::to_string((int) err);
    }

    std::cerr << "GL_" << error.c_str() << " - " << desc << " at "
              << file << ":" << line << std::endl;
    err = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
  }
}

////////////////////////////////////////////////////////////////
std::string loadTextFile(const std::string& filename)
{
  std::ifstream t(filename);
  return std::string((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
}

////////////////////////////////////////////////////////////////
bool loadShaderFile(GLenum shaderType, GLuint& shader, const std::string& filename)
{
  if (!loadShader(shaderType, shader, loadTextFile(filename)))
  {
    fprintf(stderr, "Cannot load vertex shader for %s\n", filename.c_str());
    return false;
  }
  checkErrors("loadShader " + filename);
  return true;
}

////////////////////////////////////////////////////////////////
bool loadShader(GLenum shaderType, GLuint& shader, const std::string& src)
{
  // free old shader/program
  if (shader != 0)
  {
    glDeleteShader(shader);
    checkErrors("glDeleteShader");
  }

  shader = glCreateShader(shaderType);
  checkErrors("glCreateShader");

  if (shader == 0)
  {
    return false;
  }

  static std::string common = loadTextFile("common.glsl");
  const char* c_str[] = { common.c_str(), src.c_str() };
  glShaderSource(shader, 2, c_str, nullptr);
  checkErrors("glShaderSource");

  glCompileShader(shader);
  checkErrors("glCompileShader");

  auto val = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &val);
  if (val != GL_TRUE)
  {
    char log[4096];
    GLsizei msglen;
    glGetShaderInfoLog(shader, 4096, &msglen, log);
    printf("%s\n", log);
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////
bool linkShaders(GLuint& program_id,
                 GLuint& vertex_shader_id,
                 GLuint& fragment_shader_id)
{
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  auto val = GL_FALSE;
  glGetProgramiv(program_id, GL_LINK_STATUS, &val);
  if (val != GL_TRUE)
  {
    char log[4096];
    GLsizei msglen;
    glGetProgramInfoLog(program_id, 4096, &msglen, log);
    printf("%s\n", log);
    return false;
  }

  checkErrors("linkShaders");
  printf("Link shader OK\n");

  return true;
}

} // end namespace graphics
} // end namespace kte

