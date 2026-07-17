#include "renderer.h"

// public

// renderer funcs..
    // load/init/destroy textures, shaders

// private

Renderer::TextureStatus Renderer::Texture::initialize(std::string &path) {
    return TextureStatus::SUCCESS;
}

Renderer::TextureStatus Renderer::Texture::bind(void) {
    return TextureStatus::SUCCESS;
}

Renderer::TextureStatus Renderer::Texture::destroy(void) {
    return TextureStatus::SUCCESS;
}