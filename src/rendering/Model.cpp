#include "endergfx/Model.hpp"
#include "LightUniforms.hpp"
#include "ShaderUtils.hpp"
#include "endergfx/Log.hpp"
#include <bx/math.h>

namespace endergfx {

namespace {

std::shared_ptr<Material> defaultMaterial() {
  return std::make_shared<Material>();
}

bgfx::UniformHandle textureSampler() {
  static bgfx::UniformHandle handle =
      bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
  return handle;
}

} // namespace

Model::Model(std::shared_ptr<Mesh> mesh)
    : m_mesh(mesh), m_material(defaultMaterial()),
      m_program(ShaderUtils::createDefaultProgram()), m_ownsProgram(true) {
  if (!bgfx::isValid(this->m_program)) {
    log(LogLevel::Error,
        "Model created with an invalid DEFAULT shader program");
  }
}

Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
    : m_mesh(mesh), m_material(std::move(material)),
      m_program(ShaderUtils::createDefaultProgram()), m_ownsProgram(true) {
  if (!bgfx::isValid(this->m_program)) {
    log(LogLevel::Error,
        "Model created with an invalid DEFAULT shader program");
  }
}

Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
             bgfx::ProgramHandle program)
    : m_mesh(mesh), m_material(std::move(material)), m_program(program),
      m_ownsProgram(false) {
  if (!bgfx::isValid(this->m_program)) {
    log(LogLevel::Error, "Model created with an invalid shader program");
  }
}

Model::~Model() {
  if (this->m_ownsProgram && bgfx::isValid(this->m_program)) {
    bgfx::destroy(this->m_program);
  }
}

void Model::setPosition(float x, float y, float z) {
  this->m_position = {x, y, z};
}
void Model::setRotation(float x, float y, float z) {
  this->m_rotation = {x, y, z};
}
void Model::setScale(float x, float y, float z) { this->m_scale = {x, y, z}; }

void Model::draw(bgfx::ViewId view) const {
  if (!bgfx::isValid(this->m_program)) {
    return;
  }

  float mtx[16];
  bx::mtxSRT(mtx, this->m_scale.x, this->m_scale.y, this->m_scale.z,
             bx::toRad(this->m_rotation.x), bx::toRad(this->m_rotation.y),
             bx::toRad(this->m_rotation.z), this->m_position.x,
             this->m_position.y, this->m_position.z);

  bgfx::setTransform(mtx);
  this->m_mesh->bind();
  bgfx::setTexture(0, textureSampler(), this->m_material->texture().handle());
  bgfx::setState(BGFX_STATE_DEFAULT);
  LightUniforms::applyActive(view);
  bgfx::submit(view, this->m_program);
}

} // namespace endergfx
