#include "endergfx/Renderer.hpp"
#include "LightUniforms.hpp"
#include "endergfx/Log.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_properties.h>
#include <cstdint>

namespace endergfx {

Renderer::Renderer(Window &window, unsigned int width, unsigned int height)
    : m_width(width), m_height(height) {
  SDL_PropertiesID props = SDL_GetWindowProperties(window.native());
  bgfx::PlatformData pd{};

#if BX_PLATFORM_WINDOWS
  pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER,
                                  nullptr);
  pd.type = bgfx::NativeWindowHandleType::Default;
#elif BX_PLATFORM_OSX
  pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER,
                                  nullptr);
  pd.type = bgfx::NativeWindowHandleType::Default;
#else
  const char *driver = SDL_GetCurrentVideoDriver();
  if (driver && SDL_strcmp(driver, "wayland") == 0) {
    pd.ndt = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
    pd.nwh = SDL_GetPointerProperty(
        props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    pd.type = bgfx::NativeWindowHandleType::Wayland;
  } else if (driver && SDL_strcmp(driver, "x11") == 0) {
    pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
                                    nullptr);
    pd.nwh = reinterpret_cast<void *>(static_cast<uintptr_t>(
        SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0)));
    pd.type = bgfx::NativeWindowHandleType::Default;
  } else {
    log(LogLevel::Error, std::string("Unsupported video driver: ") +
                             (driver ? driver : "unknown"));
    return;
  }
#endif

  bgfx::setPlatformData(pd);

  bgfx::Init init;
  init.type = bgfx::RendererType::Count;
  init.resolution.width = this->m_width;
  init.resolution.height = this->m_height;
  init.resolution.reset = BGFX_RESET_VSYNC;
  init.platformData = pd;

  if (!bgfx::init(init)) {
    log(LogLevel::Error, "Failed to initialize bgfx");
    return;
  }

  bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f,
                     0);
  bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(this->m_width),
                    static_cast<uint16_t>(this->m_height));

  this->m_valid = true;
}

Renderer::~Renderer() {
  if (this->m_valid) {
    bgfx::shutdown();
  }
}

void Renderer::setCamera(bgfx::ViewId view, const Camera &camera) {
  bgfx::setViewTransform(view, camera.viewMatrix(), camera.projMatrix());
}

void Renderer::setLight(bgfx::ViewId view, const Light &light) {
  LightUniforms::setActive(view, light);
}

void Renderer::beginFrame() { bgfx::touch(0); }

void Renderer::endFrame() { bgfx::frame(); }

} // namespace endergfx
