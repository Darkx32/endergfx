#include "ShaderUtils.hpp"
#include "endergfx/Log.hpp"
#include "essl/model_fs.sc.bin.h"
#include "essl/model_vs.sc.bin.h"
#include "glsl/model_fs.sc.bin.h"
#include "glsl/model_vs.sc.bin.h"
#include "spirv/model_fs.sc.bin.h"
#include "spirv/model_vs.sc.bin.h"
#include <bx/platform.h>
#if BX_PLATFORM_WINDOWS
#include "dx11/model_fs.sc.bin.h"
#include "dx11/model_vs.sc.bin.h"
#elif BX_PLATFORM_OSX
#include "metal/model_fs.sc.bin.h"
#include "metal/model_vs.sc.bin.h"
#endif
#include <bgfx/bgfx.h>

namespace endergfx::ShaderUtils {
namespace {

bgfx::ShaderHandle
createShaderForCurrentRenderer(const uint8_t *glsl, uint32_t glslSize,
                               const uint8_t *essl, uint32_t esslSize,
                               const uint8_t *spv, uint32_t spvSize
#if BX_PLATFORM_WINDOWS
                               ,
                               const uint8_t *dx11, uint32_t dx11Size
#elif BX_PLATFORM_OSX
                               ,
                               const uint8_t *mtl, uint32_t mtlSize
#endif
) {
  const uint8_t *data = nullptr;
  uint32_t size = 0;
  switch (bgfx::getRendererType()) {
  case bgfx::RendererType::OpenGL:
    data = glsl;
    size = glslSize;
    break;
  case bgfx::RendererType::OpenGLES:
    data = essl;
    size = esslSize;
    break;
  case bgfx::RendererType::Vulkan:
    data = spv;
    size = spvSize;
    break;
#if BX_PLATFORM_WINDOWS
  case bgfx::RendererType::Direct3D11:
  case bgfx::RendererType::Direct3D12:
    data = dx11;
    size = dx11Size;
    break;
#elif BX_PLATFORM_OSX
  case bgfx::RendererType::Metal:
    data = mtl;
    size = mtlSize;
    break;
#endif
  default:
    return BGFX_INVALID_HANDLE;
  }
  return bgfx::createShader(bgfx::copy(data, size));
}

} // namespace

bgfx::ProgramHandle createDefaultProgram() {
  bgfx::ShaderHandle vsh = createShaderForCurrentRenderer(
      model_vs_glsl, sizeof(model_vs_glsl), model_vs_essl,
      sizeof(model_vs_essl), model_vs_spv, sizeof(model_vs_spv)
#if BX_PLATFORM_WINDOWS
                                               ,
      model_vs_dx11, sizeof(model_vs_dx11)
#elif BX_PLATFORM_OSX
                                               ,
      model_vs_mtl, sizeof(model_vs_mtl)
#endif
  );
  bgfx::ShaderHandle fsh = createShaderForCurrentRenderer(
      model_fs_glsl, sizeof(model_fs_glsl), model_fs_essl,
      sizeof(model_fs_essl), model_fs_spv, sizeof(model_fs_spv)
#if BX_PLATFORM_WINDOWS
                                               ,
      model_fs_dx11, sizeof(model_fs_dx11)
#elif BX_PLATFORM_OSX
                                               ,
      model_fs_mtl, sizeof(model_fs_mtl)
#endif
  );

  if (!bgfx::isValid(vsh) || !bgfx::isValid(fsh)) {
    log(LogLevel::Error,
        "Failed to create default shader program (unsupported renderer?)");
    if (bgfx::isValid(vsh))
      bgfx::destroy(vsh);
    if (bgfx::isValid(fsh))
      bgfx::destroy(fsh);
    return BGFX_INVALID_HANDLE;
  }
  return bgfx::createProgram(vsh, fsh, true);
}

} // namespace endergfx::ShaderUtils
