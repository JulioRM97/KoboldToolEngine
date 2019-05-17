#include "kte/graphics/drawables/skybox.h"
#include "kte/graphics/gl_internals.h"

#include <stb_image.h>

#include "kte/game/game_manager.h"
#include "kte/graphics/program.h"
#include "kte/graphics/materials/programs/skybox_program.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
static const float kPoints[] = {
    -10.0f, 10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, -10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, -10.0f, 10.0f,
    -10.0f, -10.0f, 10.0f,

    -10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, -10.0f,
    10.0f, 10.0f, 10.0f,
    10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, 10.0f,
    -10.0f, 10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, -10.0f,
    10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f, 10.0f,
    10.0f, -10.0f, 10.0f
};

////////////////////////////////////////////////////////////////
static bool load_cube_map_side(GLenum side_target, const char* file_name)
{
  int w, h, components;
  int force_channels = 4;
  auto* image_data = stbi_load(file_name, &w, &h, &components, force_channels);
  if (!image_data)
  {
    fprintf(stderr, "ERROR: could not load %s\n", file_name);
    return false;
  }

  // non-power-of-2 dimensions check
  if ((w & (w - 1)) != 0 || (h & (h - 1)) != 0)
  {
    fprintf(stderr,
            "WARNING: image %s is not power-of-2 dimensions\n",
            file_name);
  }

  // copy image data into 'target' side of cube map
  glTexImage2D(side_target, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               image_data);
  checkErrors("glTexImage2D");

  free(image_data);

  return true;
}

////////////////////////////////////////////////////////////////
void Skybox::GPUData::load()
{
  if (cpu_version_ <= gpu_version_)
  {
    return;
  }

  uint32_t next_version = cpu_version_;

  // generate a cube-map texture to hold all the sides
  if (!texture_id)
  {
    glGenTextures(1, &texture_id);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

  // load each image and copy into a side of the cube-map texture
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, nz.c_str());
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, pz.c_str());
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, py.c_str());
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, ny.c_str());
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, nx.c_str());
  load_cube_map_side(GL_TEXTURE_CUBE_MAP_POSITIVE_X, px.c_str());
  checkErrors("load_cube_map_side");

  // format cube map texture
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  checkErrors("glTexParameteri");

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  checkErrors("glGenerateMipmap");

  if (!vao_id)
  {
    glGenVertexArrays(1, &vao_id);
  }
  glBindVertexArray(vao_id);

  if (!vbo_id)
  {
    glGenBuffers(1, &vbo_id);
  }
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

  glBufferData(GL_ARRAY_BUFFER, sizeof(kPoints), &kPoints, GL_STATIC_DRAW);
  checkErrors("glBufferData");

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  checkErrors("glVertexAttribPointer");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Skybox::GPUData::unload()
{
  if (vbo_id)
  {
    glDeleteBuffers(1, &vbo_id);
  }

  if (vao_id)
  {
    glDeleteVertexArrays(1, &vao_id);
  }

  if (texture_id)
  {
    glDeleteTextures(1, &texture_id);
  }
}

////////////////////////////////////////////////////////////////
Skybox::Skybox()
    : data_(new GPUData()),
      program_(new kte::graphics::SkyboxProgram())
{
}

////////////////////////////////////////////////////////////////
Skybox::~Skybox() = default;

////////////////////////////////////////////////////////////////
void Skybox::loadFiles(std::string nx,
                       std::string ny,
                       std::string nz,
                       std::string px,
                       std::string py,
                       std::string pz)
{
  data_->nx = nx;
  data_->ny = ny;
  data_->nz = nz;
  data_->px = px;
  data_->py = py;
  data_->pz = pz;

  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
void Skybox::load()
{
  program_->load();
  data_->load();
}

////////////////////////////////////////////////////////////////
void Skybox::draw() const
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, data_->texture_id);
  checkErrors("glBindTexture");

  glBindVertexArray(data_->vao_id);
  checkErrors("glBindVertexArray");

  glDrawArrays(GL_TRIANGLES, 0, 36);
  checkErrors("glDrawArrays");

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////
void Skybox::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&Skybox::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
Program& Skybox::program() const
{
  return *program_;
}

} // end namespace graphics
} // end namespace kte

