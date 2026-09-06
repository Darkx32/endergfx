#include <endergfx/endergfx.hpp>
#include <vector>

int main() {
  endergfx::WindowConfig config;
  config.title = "endergfx - model loading";
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

  auto meshes = endergfx::ModelLoader::loadOBJ("assets/suzanne.obj");
  if (meshes.empty()) {
    return 1;
  }
  auto meshPtr = std::make_shared<endergfx::Mesh>(std::move(meshes[0]));
  endergfx::Model model(meshPtr);

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
    model.setRotation(0.0f, angle * 30.0f, 0.0f);

    renderer.setCamera(0, camera);
    renderer.beginFrame();
    model.draw(0);
    renderer.endFrame();
  }

  return 0;
}
