#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <memory>

namespace kte
{
namespace graphics
{

// Forward declarations
class Texture;

class GenerateMipmapCommand : public DisplayListCommand
{
 public:
  explicit GenerateMipmapCommand(const std::shared_ptr<Texture> texture);

  ~GenerateMipmapCommand() override;

  void exec() const override;

 private:
  const std::shared_ptr<Texture> texture_;

};

}//End namespace graphics
}//End namespace kte


