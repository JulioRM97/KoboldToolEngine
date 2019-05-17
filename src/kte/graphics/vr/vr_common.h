#pragma once

#include <memory>
#include <vector>

#ifdef OCULUS

#include <Extras/OVR_Math.h>
#include <OVR_CAPI_GL.h>

namespace kte
{
namespace graphics
{

class Framebuffer;
class TextureAttachment;

////////////////////////////////////////////////////////////////
static bool check_oculus_error(ovrResult result, const char* message)
{
  if (OVR_FAILURE(result))
  {
    ovrErrorInfo error;
    ovr_GetLastErrorInfo(&error);
    fprintf(stderr, "WARN: %s: %s\n", message, error.ErrorString);
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)
#define assert_vr(result) if (check_oculus_error(result, __FILE__ ":" S__LINE__)) return false;

} // end namespace graphics
} // end namespace kte

#endif

