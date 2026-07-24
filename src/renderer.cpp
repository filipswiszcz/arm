#include "renderer.h"

// public

void Renderer::initialize(void) {}

void Renderer::draw(void) {
    this->cmd_counter = 0;
    this->inst_counter = 0;
}

void Renderer::terminate(void) {}

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