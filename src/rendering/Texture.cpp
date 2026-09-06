#include "endergfx/Texture.hpp"
#include "bgfx/bgfx.h"
#include "endergfx/Log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace endergfx {

Texture::Texture(bgfx::TextureHandle handle) : m_handle(handle) {}

Texture::Texture(const std::string &path) {
  int width, height, channels;
  stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 4);

  if (!data) {
    log(LogLevel::Error, "Texture: failed to load image: " + path);
    return;
  }

  const bgfx::Memory *mem =
      bgfx::copy(data, static_cast<uint32_t>(width * height * 4));
  stbi_image_free(data);

  this->m_handle = bgfx::createTexture2D(
      static_cast<uint16_t>(width), static_cast<uint16_t>(height), false, 1,
      bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE, mem);

  if (!bgfx::isValid(this->m_handle)) {
    log(LogLevel::Error, "Texture: failed to create GPU texture for: " + path);
  }
}

Texture Texture::createSolidColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  uint8_t pixel[4] = {r, g, b, a};
  const bgfx::Memory *mem = bgfx::copy(pixel, 4);

  bgfx::TextureHandle handle = bgfx::createTexture2D(
      1, 1, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE, mem);

  return Texture(handle);
}

Texture::~Texture() {
  if (bgfx::isValid(this->m_handle)) {
    bgfx::destroy(this->m_handle);
    this->m_handle = BGFX_INVALID_HANDLE;
  }
}

Texture::Texture(Texture &&other) noexcept : m_handle(other.m_handle) {
  other.m_handle = BGFX_INVALID_HANDLE;
}

Texture &Texture::operator=(Texture &&other) noexcept {
  if (this != &other) {
    if (bgfx::isValid(this->m_handle)) {
      bgfx::destroy(this->m_handle);
    }
    this->m_handle = other.m_handle;
    other.m_handle = BGFX_INVALID_HANDLE;
  }
  return *this;
}

} // namespace endergfx
