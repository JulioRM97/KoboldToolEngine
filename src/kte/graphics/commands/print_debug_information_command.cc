#include "kte/graphics/commands/print_debug_information_command.h"

#include <cstdio>

#include "kte/timer.h"

namespace kte
{
namespace graphics
{

////////////////////////////////////////////////////////////////
static struct PrintDebugInformationCommand_Data
{
  PrintDebugInformationCommand_Data();

  Timer timer;

  uint32_t frames = 0;

  float fps = 0;
} data_;

////////////////////////////////////////////////////////////////
PrintDebugInformationCommand_Data::PrintDebugInformationCommand_Data()
{
  timer.start();
}

////////////////////////////////////////////////////////////////
PrintDebugInformationCommand::PrintDebugInformationCommand() = default;

////////////////////////////////////////////////////////////////
PrintDebugInformationCommand::~PrintDebugInformationCommand() = default;

////////////////////////////////////////////////////////////////
void PrintDebugInformationCommand::exec() const
{
  if (++(data_.frames) % 150 == 0)
  {
    data_.fps = 150 / data_.timer.elapsed(true);
    printf("FPS: %.2f\n", data_.fps);
    fflush(stdout);
  }
}

} // end namespace graphics
} // end namespace kte

