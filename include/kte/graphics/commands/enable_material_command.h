#pragma once

#include "kte/graphics/display/display_list_command.h"

#include <memory>

#include "kte/types.h"

namespace kte
{
namespace graphics
{

// Forward declarations
class Material;

class EnableMaterialCommand : public DisplayListCommand
{
 public:
  explicit EnableMaterialCommand(const std::shared_ptr<Material> material,
                                 RenderMode mode);

  ~EnableMaterialCommand() override;

  void exec() const override;

 private:
  const std::shared_ptr<Material> material_;

  RenderMode mode_;
};

} // end namespace graphics
} // end namespace kte

