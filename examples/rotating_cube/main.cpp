#include <endergfx/endergfx.hpp>
#include <memory>
#include <vector>

int main() {
  endergfx::WindowConfig config;
  config.title = "endergfx - rotating cube";
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

  std::vector<endergfx::Vertex> vertices = {
      {.x = -1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = 1.0f,
       .abgr = 0xff000000},
      {.x = 1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = 1.0f,
       .abgr = 0xff0000ff},
      {.x = -1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = 1.0f,
       .abgr = 0xff00ff00},
      {.x = 1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = 1.0f,
       .abgr = 0xff00ffff},
      {.x = -1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = -1.0f,
       .abgr = 0xffff0000},
      {.x = -1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = -1.0f,
       .abgr = 0xffffff00},
      {.x = 1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = -1.0f,
       .abgr = 0xffff00ff},
      {.x = 1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 0.0f,
       .nz = -1.0f,
       .abgr = 0xffffffff},
      {.x = -1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = -1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xff000000},
      {.x = -1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = -1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xff00ff00},
      {.x = -1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = -1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xffff0000},
      {.x = -1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = -1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xffffff00},
      {.x = 1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = 1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xff0000ff},
      {.x = 1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = 1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xffff00ff},
      {.x = 1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = 1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xff00ffff},
      {.x = 1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = 1.0f,
       .ny = 0.0f,
       .nz = 0.0f,
       .abgr = 0xffffffff},
      {.x = -1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 1.0f,
       .nz = 0.0f,
       .abgr = 0xff000000},
      {.x = -1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 1.0f,
       .nz = 0.0f,
       .abgr = 0xffff0000},
      {.x = 1.0f,
       .y = 1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = 1.0f,
       .nz = 0.0f,
       .abgr = 0xff0000ff},
      {.x = 1.0f,
       .y = 1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = 1.0f,
       .nz = 0.0f,
       .abgr = 0xffff00ff},
      {.x = -1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = -1.0f,
       .nz = 0.0f,
       .abgr = 0xff00ff00},
      {.x = 1.0f,
       .y = -1.0f,
       .z = 1.0f,
       .nx = 0.0f,
       .ny = -1.0f,
       .nz = 0.0f,
       .abgr = 0xff00ffff},
      {.x = -1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = -1.0f,
       .nz = 0.0f,
       .abgr = 0xffffff00},
      {.x = 1.0f,
       .y = -1.0f,
       .z = -1.0f,
       .nx = 0.0f,
       .ny = -1.0f,
       .nz = 0.0f,
       .abgr = 0xffffffff},
  };

  std::vector<uint16_t> indices = {
      0,  1,  2,  1,  3,  2,  4,  5,  6,  6,  5,  7,  8,  9,  10, 10, 9,  11,
      12, 13, 14, 13, 15, 14, 16, 17, 18, 17, 19, 18, 20, 21, 22, 22, 21, 23,
  };

  auto cubeMesh = std::make_shared<endergfx::Mesh>(vertices, indices);
  endergfx::Model cube(cubeMesh);

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
    renderer.beginFrame();
    cube.draw(0);
    renderer.endFrame();
  }

  return 0;
}
