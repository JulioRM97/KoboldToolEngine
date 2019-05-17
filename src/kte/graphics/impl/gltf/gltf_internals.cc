// Define these only in *one* .cc file, this one.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "gltf_internals.h"

#include "kte/graphics/gl_internals.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext.hpp>

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
GLint size(const tinygltf::Accessor& accessor)
{
  if (accessor.type == TINYGLTF_TYPE_SCALAR)
  {
    return 1;
  }
  else if (accessor.type == TINYGLTF_TYPE_VEC2)
  {
    return 2;
  }
  else if (accessor.type == TINYGLTF_TYPE_VEC3)
  {
    return 3;
  }
  else if (accessor.type == TINYGLTF_TYPE_VEC4)
  {
    return 4;
  }
  else
  {
    fprintf(stderr, "Invalid accessor type %d\n", accessor.type);
    return -1;
  }
}

////////////////////////////////////////////////////////////////
GLenum glMode(const tinygltf::Primitive& primitive)
{
  if (primitive.mode == TINYGLTF_MODE_TRIANGLES)
  {
    return GL_TRIANGLES;
  }
  else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP)
  {
    return GL_TRIANGLE_STRIP;
  }
  else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN)
  {
    return GL_TRIANGLE_FAN;
  }
  else if (primitive.mode == TINYGLTF_MODE_POINTS)
  {
    return GL_POINTS;
  }
  else if (primitive.mode == TINYGLTF_MODE_LINE)
  {
    return GL_LINES;
  }
  else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP)
  {
    return GL_LINE_LOOP;
  }
  else
  {
    fprintf(stderr, "Invalid primitive mode %d\n", primitive.mode);
    return GL_INVALID_ENUM;
  }
}

////////////////////////////////////////////////////////////////
GLenum gltfToGLenum(int gltf_value)
{
  switch (gltf_value)
  {
    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
      return GL_NEAREST_MIPMAP_NEAREST;

    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
      return GL_LINEAR_MIPMAP_NEAREST;

    case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
      return GL_NEAREST_MIPMAP_LINEAR;

    case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
      return GL_LINEAR_MIPMAP_LINEAR;

    case TINYGLTF_TEXTURE_FILTER_NEAREST:
      return GL_NEAREST;

    case TINYGLTF_TEXTURE_FILTER_LINEAR:
      return GL_LINEAR;

    case TINYGLTF_TEXTURE_WRAP_REPEAT:
      return GL_REPEAT;

    case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
      return GL_CLAMP_TO_EDGE;

    case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
      return GL_MIRRORED_REPEAT;

    default:
      return GL_INVALID_ENUM;
  }
}

////////////////////////////////////////////////////////////////
void gltfTexParameteri(GLenum name, int gltf_value)
{
  auto gl_value = gltfToGLenum(gltf_value);

  if (gl_value != GL_INVALID_ENUM)
  {
    glTexParameteri(GL_TEXTURE_2D, name, gl_value);
    checkErrors("glTexParameteri");
  }
}

////////////////////////////////////////////////////////////////
glm::vec4 gltfParamToVec4(const tinygltf::Parameter& parameter)
{
  auto& vector = parameter.number_array;

  switch (vector.size())
  {
    case 4:
      return glm::make_vec4(vector.data());

    case 3:
      return glm::vec4(vector[0], vector[1], vector[2], 1.0f);

    case 2:
      return glm::vec4(vector[0], vector[1], 1.0f, 1.0f);

    case 1:
      return glm::vec4(static_cast<float>(vector[0]));

    default:
      return glm::vec4(1.0f);
  }
}

} // end namespace graphics
} // end namespace kte

