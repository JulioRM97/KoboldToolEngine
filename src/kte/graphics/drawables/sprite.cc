#include "kte/graphics/drawables/sprite.h"
#include "kte/graphics/gl_internals.h"

#include <iostream>

#include "kte/game/game_manager.h"
#include "kte/graphics/material.h"
#include "kte/graphics/materials/programs/sprite_renderer_program.h"
#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
static const float kPoints[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

////////////////////////////////////////////////////////////////
void Sprite::GPUData::load()
{
  if (cpu_version_ <= gpu_version_)
  {
    // Sprite is up to date... do not update
    return;
  }

  uint32_t next_version = cpu_version_;

  if (!vao_id)
  {
    glGenVertexArrays(1, &vao_id);
  }
  glBindVertexArray(vao_id);
  checkErrors("glBindVertexArray");

  GLuint vbo_id;
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  checkErrors("glBindBuffer");

  glBufferData(GL_ARRAY_BUFFER, sizeof(kPoints), &kPoints, GL_STATIC_DRAW);
  checkErrors("glBufferData");

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  checkErrors("glVertexAttribPointer");

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  gpu_version_ = next_version;
}

////////////////////////////////////////////////////////////////
void Sprite::GPUData::unload()
{
  if (vao_id)
  {
    glDeleteVertexArrays(1, &vao_id);
    vao_id = 0;
  }
}

////////////////////////////////////////////////////////////////
Sprite::Sprite()
    : data_(new GPUData()),
      program_(new SpriteRendererProgram())
{
  data_->cpu_version_++;
}

////////////////////////////////////////////////////////////////
Sprite::~Sprite()
{
  unload();
}

////////////////////////////////////////////////////////////////
void Sprite::load()
{
  program_->load();
  texture_->load();
  data_->load();
}

////////////////////////////////////////////////////////////////
void Sprite::draw() const
{
  program_->applySpriteParams(*this);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_->internal_id());
  checkErrors("glBindTexture");

  glBindVertexArray(data_->vao_id);
  checkErrors("glBindVertexArray");

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  checkErrors("glDrawArrays");

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////
void Sprite::unload()
{
  auto& scheduler = kte::GameManager::Instance().scheduler_;
  scheduler.schedule(std::bind(&Sprite::GPUData::unload, data_), true);
}

////////////////////////////////////////////////////////////////
Program& Sprite::program() const
{
  return *program_;
}

} // end namespace graphics
} // end namespace kte

