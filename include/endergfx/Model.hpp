#pragma once
#include "endergfx/Material.hpp"
#include "endergfx/Mesh.hpp"
#include <bgfx/bgfx.h>
#include <memory>

namespace endergfx {

class Model {
public:
  explicit Model(std::shared_ptr<Mesh> mesh);
  Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
  Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
        bgfx::ProgramHandle program);

  ~Model();

  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

  void setPosition(float x, float y, float z);
  void setRotation(float x, float y, float z);
  void setScale(float x, float y, float z);

  void draw(bgfx::ViewId view) const;

private:
  std::shared_ptr<Mesh> m_mesh;
  std::shared_ptr<Material> m_material;
  bgfx::ProgramHandle m_program;
  bool m_ownsProgram;

  struct {
    float x, y, z;
  } m_position{0, 0, 0};
  struct {
    float x, y, z;
  } m_rotation{0, 0, 0};
  struct {
    float x, y, z;
  } m_scale{1, 1, 1};
};

} // namespace endergfx
