#pragma once
#include "endergfx/Camera.hpp"
#include "endergfx/Light.hpp"
#include "endergfx/Window.hpp"
#include <bgfx/bgfx.h>

namespace endergfx {

class Renderer {
public:
  Renderer(Window &window, unsigned int width, unsigned int height);
  ~Renderer();

  void beginFrame();
  void endFrame();

  void setCamera(bgfx::ViewId view, const Camera &camera);
  void setLight(bgfx::ViewId view, const Light &light);

  bool isValid() const { return this->m_valid; }

private:
  unsigned int m_width;
  unsigned int m_height;
  bool m_valid = false;
};

} // namespace endergfx
