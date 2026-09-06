#include "endergfx/Material.hpp"
#include "endergfx/Mesh.hpp"
#include <endergfx/endergfx.hpp>
#include <memory>
#include <vector>

int main() {
  endergfx::WindowConfig config;
  config.title = "endergfx - textured cube";
  config.width = 1280;
  config.height = 720;

  endergfx::Window window(config);
  endergfx::Input input;
  endergfx::Renderer renderer(window, config.width, config.height);

  endergfx::Camera camera;
  camera.setPosition(0.0f, 0.0f, -5.0f);
  camera.lookAt(0.0f, 0.0f, 0.0f);
  camera.setPerspective(60.0f,
                        static_cast<float>(config.width) /
                            static_cast<float>(config.height),
                        0.1f, 100.0f);

  endergfx::Light light;
  light.setDirection(0.3f, 0.7f, -0.5f);
  light.setAmbient(0.3f);

  // clang-format off
  std::vector<endergfx::Vertex> vertices = {
      {.x=-1,.y= 1,.z= 1, .nx=0,.ny=0,.nz= 1, .u=0,.v=0, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z= 1, .nx=0,.ny=0,.nz= 1, .u=1,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z= 1, .nx=0,.ny=0,.nz= 1, .u=0,.v=1, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z= 1, .nx=0,.ny=0,.nz= 1, .u=1,.v=1, .abgr=0xffffffff},
      {.x=-1,.y= 1,.z=-1, .nx=0,.ny=0,.nz=-1, .u=1,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z=-1, .nx=0,.ny=0,.nz=-1, .u=1,.v=1, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z=-1, .nx=0,.ny=0,.nz=-1, .u=0,.v=0, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z=-1, .nx=0,.ny=0,.nz=-1, .u=0,.v=1, .abgr=0xffffffff},
      {.x=-1,.y= 1,.z= 1, .nx=-1,.ny=0,.nz=0, .u=1,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z= 1, .nx=-1,.ny=0,.nz=0, .u=1,.v=1, .abgr=0xffffffff},
      {.x=-1,.y= 1,.z=-1, .nx=-1,.ny=0,.nz=0, .u=0,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z=-1, .nx=-1,.ny=0,.nz=0, .u=0,.v=1, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z= 1, .nx=1,.ny=0,.nz=0, .u=0,.v=0, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z=-1, .nx=1,.ny=0,.nz=0, .u=1,.v=0, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z= 1, .nx=1,.ny=0,.nz=0, .u=0,.v=1, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z=-1, .nx=1,.ny=0,.nz=0, .u=1,.v=1, .abgr=0xffffffff},
      {.x=-1,.y= 1,.z= 1, .nx=0,.ny=1,.nz=0, .u=0,.v=1, .abgr=0xffffffff},
      {.x=-1,.y= 1,.z=-1, .nx=0,.ny=1,.nz=0, .u=0,.v=0, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z= 1, .nx=0,.ny=1,.nz=0, .u=1,.v=1, .abgr=0xffffffff},
      {.x= 1,.y= 1,.z=-1, .nx=0,.ny=1,.nz=0, .u=1,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z= 1, .nx=0,.ny=-1,.nz=0, .u=0,.v=0, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z= 1, .nx=0,.ny=-1,.nz=0, .u=1,.v=0, .abgr=0xffffffff},
      {.x=-1,.y=-1,.z=-1, .nx=0,.ny=-1,.nz=0, .u=0,.v=1, .abgr=0xffffffff},
      {.x= 1,.y=-1,.z=-1, .nx=0,.ny=-1,.nz=0, .u=1,.v=1, .abgr=0xffffffff},
  };
  // clang-format on

  std::vector<uint16_t> indices = {
      0,  1,  2,  1,  3,  2,  4,  5,  6,  6,  5,  7,  8,  9,  10, 10, 9,  11,
      12, 13, 14, 13, 15, 14, 16, 17, 18, 17, 19, 18, 20, 21, 22, 22, 21, 23,
  };

  auto cubeMesh = std::make_shared<endergfx::Mesh>(vertices, indices);

  std::string basePath = endergfx::PathUtils::getExecutableDir();
  endergfx::Texture texture(basePath + "assets/dirt.png");
  auto material = std::make_shared<endergfx::Material>(std::move(texture));
  endergfx::Model cube(cubeMesh, material);

  float angle = 0.0f;

  SDL_Event event;
  while (window.isRunning()) {
    input.resetFrameState();
    while (SDL_PollEvent(&event)) {
      window.handleEvent(event);
      input.handleEvent(event);
    }

    if (input.isKeyDown(SDL_SCANCODE_ESCAPE)) {
      break;
    }

    angle += 0.01f;
    cube.setRotation(angle * 20.0f, angle * 40.0f, 0.0f);

    renderer.setCamera(0, camera);
    renderer.setLight(0, light);
    renderer.beginFrame();
    cube.draw(0);
    renderer.endFrame();
  }

  return 0;
}
