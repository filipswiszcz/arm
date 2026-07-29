#include "renderer.h"

namespace Renderer {

// public

void Renderer::initialize(void) {
    glGenBuffers(1, &this->gpu.mesh.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.mesh.ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GPUInstanceData_t) * this->CHUNK_SIZE, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->gpu.mesh.ubo);

    glGenVertexArrays(1, &this->gpu.line.vao);
    glGenBuffers(1, &this->gpu.line.vbo);
    glBindVertexArray(this->gpu.line.vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->gpu.line.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GPULineData_t) * this->MAX_LINES, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) (sizeof(v3)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) (sizeof(v3) * 2));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) ((sizeof(v3) * 2) + sizeof(v4)));
    glEnableVertexAttribArray(3);
}

void Renderer::read_shaders(const char *path) {}

void Renderer::read_textures(const char *path) {}

void Renderer::read_meshes(const char *path) {}

void Renderer::read_materials(const char *path) {}

void Renderer::push_cmd(meID mesh, maID material, m4 transform, v4 tint) {

    this->instances[this->inst_counter] = {transform, tint};

    u64 key = 0;
    key |= ((u64) RendererPass::OPAQUE << 62);
    key |= ((u64) this->materials[material].id << 48);
    key |= ((u64) material << 32);
    key |= ((u64) mesh << 16);

    RendererCommand_t command = {
        .key = key,
        .type = RendererCommandType::MESH,
        .mesh = mesh,
        .material = material,
        .index = this->inst_counter
    };

    this->commands[this->cmd_counter++] = command;
    this->inst_counter++;
}

void Renderer::push_cmd(v3 start, v3 end, v4 color, f32 thickness) {

    this->lines[this->line_counter] = {start, end, color, thickness};

    u64 key = 0;
    key |= ((u64) RendererPass::DEBUG << 62);

    RendererCommand_t command = {
        .key = key,
        .type = RendererCommandType::LINE,
        .index = this->line_counter
    };

    this->commands[this->cmd_counter++] = command;
    this->line_counter++;
}

void Renderer::push_cmd(const char *text, v2 pos, f32 scale, v4 color) {}

void Renderer::push_cmd(v4 color) {
    u64 key = 0;
    key |= ((u64) RendererPass::DEPTH_PREPASS << 62);

    RendererCommand_t command = {
        .key = key,
        .type = RendererCommandType::GRID
    };

    this->commands[this->cmd_counter++] = command;
}

void Renderer::draw(void) {
    if (this->cmd_counter == 0) return;

    GPUInstanceData_t chunk[this->CHUNK_SIZE];
    u32 chunks = 0, chunkstances = 0;

    shID shader = 0xFFFF;
    maID material = 0xFFFF;

    for (u32 i = 0; i < this->cmd_counter; i++) {
        RendererCommand_t *command = &this->commands[i];
        switch (command->type) {
            case RendererCommandType::MESH: {
                if (chunkstances >= this->CHUNK_SIZE) {
                    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.mesh.ubo);
                    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GPUInstanceData_t) * chunkstances, chunk);
                    chunkstances = 0;
                    chunks++;
                }

                chunk[chunkstances] = this->instances[command->index];

                if (this->materials[command->material].id != shader) {
                    shader = this->materials[command->material].id;
                    this->shaders[shader].use();
                }

                this->shaders[shader].set_uint("u_Index", chunkstances);

                glBindVertexArray(this->meshes[command->mesh].vao);
                glDrawElements(GL_TRIANGLES, this->meshes[command->mesh].counter, GL_UNSIGNED_INT, 0);

                chunkstances++;
                break;
            }
            case RendererCommandType::LINE: {
                // GPULineData_t *line = &this->lines[this->cmd_counter];
                break;
            }
        }
    }

    this->cmd_counter = 0;
    this->inst_counter = 0;
    this->line_counter = 0;
}

void Renderer::terminate(void) {}

// private

TextureStatus Texture::initialize(std::string path) {
    return TextureStatus::SUCCESS;
}

TextureStatus Texture::bind(void) {
    return TextureStatus::SUCCESS;
}

TextureStatus Texture::destroy(void) {
    return TextureStatus::SUCCESS;
}

}