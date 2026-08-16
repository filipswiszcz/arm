#include "renderer.h"

namespace Renderer {

// public

void Renderer::initialize(void) {
    // camera
    glGenBuffers(1, &this->gpu.camera.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.camera.ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GPUCameraData_t), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->gpu.camera.ubo);

    // grid
    glGenVertexArrays(1, &this->gpu.grid.vao);

    // mesh
    glGenBuffers(1, &this->gpu.mesh.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.mesh.ubo);
    glBufferData(GL_UNIFORM_BUFFER, this->CHUNK_SIZE * sizeof(GPUInstanceData_t), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->gpu.mesh.ubo);

    // text..

    // line
    glGenVertexArrays(1, &this->gpu.line.vao);
    glGenBuffers(1, &this->gpu.line.vbo);
    glBindVertexArray(this->gpu.line.vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->gpu.line.vbo);
    glBufferData(GL_ARRAY_BUFFER, this->MAX_LINES * sizeof(GPULineData_t), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) (sizeof(v3)));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) (2 * sizeof(v3)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPULineData_t), (void*) ((2 * sizeof(v3)) + sizeof(v4)));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    // point
    glGenVertexArrays(1, &this->gpu.dot.vao);
    glGenBuffers(1, &this->gpu.dot.vbo);
    glBindVertexArray(this->gpu.dot.vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->gpu.dot.vbo);
    glBufferData(GL_ARRAY_BUFFER, this->MAX_DOTS * sizeof(GPUDotData_t), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUDotData_t), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribDivisor(0, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GPUDotData_t), (void*) (sizeof(v3)));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUDotData_t), (void*) (sizeof(v3) + sizeof(v4)));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
}

void Renderer::read_shaders(const char *path) { // temp solution
    // const std::filesystem::path directory{"res/shader"};
    // for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(directory)) {
    //     if (!entry.is_regular_file()) continue;
    //     std::cout << "file" << std::endl;
    // }

    std::string grid_paths[2] = {"res/shader/grid.vs", "res/shader/grid.fs"};
    std::string mesh_paths[2] = {"res/shader/mesh.vs", "res/shader/mesh.fs"};
    std::string line_paths[2] = {"res/shader/line.vs", "res/shader/line.fs"};
    std::string dot_paths[2] = {"res/shader/dot.vs", "res/shader/dot.fs"};

    this->shaders[DEBUG_RENDERER_GRID_SHADER_ID] = Shader();
    this->shaders[DEBUG_RENDERER_GRID_SHADER_ID].initialize(grid_paths);

    this->shaders[DEBUG_RENDERER_MESH_SHADER_ID] = Shader();
    this->shaders[DEBUG_RENDERER_MESH_SHADER_ID].initialize(mesh_paths);

    this->shaders[DEBUG_RENDERER_LINE_SHADER_ID] = Shader();
    this->shaders[DEBUG_RENDERER_LINE_SHADER_ID].initialize(line_paths);

    this->shaders[DEBUG_RENDERER_DOT_SHADER_ID] = Shader();
    this->shaders[DEBUG_RENDERER_DOT_SHADER_ID].initialize(dot_paths);

    glUniformBlockBinding(
        this->shaders[DEBUG_RENDERER_GRID_SHADER_ID].get_program(), 
        glGetUniformBlockIndex(this->shaders[DEBUG_RENDERER_GRID_SHADER_ID].get_program(), "u_Camera"), 1
    );

    glUniformBlockBinding(
        this->shaders[DEBUG_RENDERER_MESH_SHADER_ID].get_program(), 
        glGetUniformBlockIndex(this->shaders[DEBUG_RENDERER_MESH_SHADER_ID].get_program(), "u_Camera"), 1
    );

    glUniformBlockBinding(
        this->shaders[DEBUG_RENDERER_MESH_SHADER_ID].get_program(), 
        glGetUniformBlockIndex(this->shaders[DEBUG_RENDERER_MESH_SHADER_ID].get_program(), "u_Instances"), 0
    );

    glUniformBlockBinding(
        this->shaders[DEBUG_RENDERER_LINE_SHADER_ID].get_program(), 
        glGetUniformBlockIndex(this->shaders[DEBUG_RENDERER_LINE_SHADER_ID].get_program(), "u_Camera"), 1
    );

    glUniformBlockBinding(
        this->shaders[DEBUG_RENDERER_DOT_SHADER_ID].get_program(), 
        glGetUniformBlockIndex(this->shaders[DEBUG_RENDERER_DOT_SHADER_ID].get_program(), "u_Camera"), 1
    );
}

void Renderer::read_textures(const char *path) {}

void Renderer::read_meshes(const char *path) {}

void Renderer::read_materials(const char *path) {}

void Renderer::push_cmd(v4 color) {
    u64 key = 0;
    key |= ((u64) RendererPass::DEPTH_PREPASS << 60);

    // RendererCommand_t command = {
    //     .key = key,
    //     .type = RendererCommandType::GRID
    // };

    RendererCommand_t command = {
        key,
        RendererCommandType::GRID
    };

    this->commands[this->cmd_counter++] = command;
}

void Renderer::push_cmd(meID mesh, maID material, m4 transform, v4 tint) {

    this->instances[this->inst_counter] = {transform, tint};

    u64 key = 0;
    key |= ((u64) RendererPass::OPAQUE << 60);
    key |= ((u64) this->materials[material].id << 44);
    key |= ((u64) material << 28);
    key |= ((u64) mesh << 12);

    // RendererCommand_t command = {
    //     .key = key,
    //     .type = RendererCommandType::MESH,
    //     .mesh = mesh,
    //     .material = material,
    //     .index = this->inst_counter
    // };

    RendererCommand_t command = {
        key,
        RendererCommandType::MESH,
        mesh,
        material,
        this->inst_counter
    };

    this->commands[this->cmd_counter++] = command;
    this->inst_counter++;
}

void Renderer::push_cmd(const char *text, v2 pos, f32 scale, v4 color) {}

void Renderer::push_cmd(v3 start, v3 end, v4 color, f32 thickness) {

    this->lines[this->line_counter] = {start, end, color, thickness};

    u64 key = 0;
    key |= ((u64) RendererPass::DEBUG << 60);

    // RendererCommand_t command = {
    //     .key = key,
    //     .type = RendererCommandType::LINE,
    //     .index = this->line_counter
    // };

    // RendererCommand_t command = {
    //     key,
    //     RendererCommandType::LINE,
    //     this->line_counter
    // };

    RendererCommand_t command = {};
    command.key = key;
    command.type = RendererCommandType::LINE;
    command.index = this->line_counter;

    this->commands[this->cmd_counter++] = command;
    this->line_counter++;
}

void Renderer::push_cmd(v3 pos, v4 color, f32 thickness) {

    this->dots[this->dot_counter] = {pos, color, thickness};

    u64 key;
    key |= ((u64) RendererPass::DEBUG << 60);

    RendererCommand_t command = {};
    command.key = key;
    command.type = RendererCommandType::DOT;
    command.index = this->dot_counter;

    this->commands[this->cmd_counter++] = command;
    this->dot_counter++;
}

// void Renderer::draw(void) {
void Renderer::draw(m4 view_proj, v3 cam_pos) {
    if (this->cmd_counter == 0) return;

    GPUInstanceData_t chunk[this->CHUNK_SIZE];
    u32 chunks = 0, chunkstances = 0;

    // GPUCameraData_t camera = {.view_proj = view_proj, .pos = cam_pos, .padd = 0.0f};
    GPUCameraData_t camera = {view_proj, cam_pos, 0.0f};
    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.camera.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GPUCameraData_t), &camera);

    std::sort(this->commands, this->commands + this->cmd_counter, 
        [](const RendererCommand_t &a, const RendererCommand_t &b) {
            return a.key < b.key;
    });

    shID shader = 0xFFFF;
    maID material = 0xFFFF;

    for (u32 i = 0; i < this->cmd_counter; i++) {
        RendererCommand_t *command = &this->commands[i];
        switch (command->type) {
            case RendererCommandType::GRID: {
                this->shaders[DEBUG_RENDERER_GRID_SHADER_ID].use();

                glBindVertexArray(this->gpu.grid.vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // std::cout << "GRID" << std::endl;

                shader = 0xFFFF;
                break;
            }
            case RendererCommandType::MESH: {
                if (chunkstances >= this->CHUNK_SIZE) {
                    glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.mesh.ubo);
                    glBufferSubData(GL_UNIFORM_BUFFER, 0, chunkstances * sizeof(GPUInstanceData_t), chunk);
                    chunkstances = 0;
                    chunks++;
                }

                chunk[chunkstances] = this->instances[command->index];

                glBindBuffer(GL_UNIFORM_BUFFER, this->gpu.mesh.ubo);
                glBufferSubData(GL_UNIFORM_BUFFER, chunkstances * sizeof(GPUInstanceData_t), sizeof(GPUInstanceData_t), &chunk[chunkstances]);

                if (this->materials[command->material].id != shader) {
                    shader = this->materials[command->material].id;
                    this->shaders[shader].use();
                }

                this->shaders[shader].set_uint("u_Index", chunkstances);

                glBindVertexArray(this->meshes[command->mesh].vao);
                glDrawElements(GL_TRIANGLES, this->meshes[command->mesh].counter, GL_UNSIGNED_INT, 0);

                // std::cout << "MESH" << std::endl;

                chunkstances++;
                break;
            }
            case RendererCommandType::LINE: {
                if ((this->cmd_counter - 1) == i || this->commands[i + 1].type != RendererCommandType::LINE) {
                    this->shaders[DEBUG_RENDERER_LINE_SHADER_ID].use();

                    glBindVertexArray(this->gpu.line.vao);
                    glBindBuffer(GL_ARRAY_BUFFER, this->gpu.line.vbo);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, this->line_counter * sizeof(GPULineData_t), this->lines);

                    glLineWidth(this->lines[command->index].thickness); // does it even work somewhere?

                    glDrawArraysInstanced(GL_LINES, 0, 2, this->line_counter);

                    // std::cout << "LINE" << std::endl;

                    glLineWidth(1.0f);
                    shader = 0xFFFF;
                    break;
                }
            }
            case RendererCommandType::DOT: {
                if ((this->cmd_counter - 1) == i || this->commands[i + 1].type != RendererCommandType::DOT) {
                    this->shaders[DEBUG_RENDERER_DOT_SHADER_ID].use();

                    glBindVertexArray(this->gpu.dot.vao);
                    glBindBuffer(GL_ARRAY_BUFFER, this->gpu.dot.vbo);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, this->dot_counter * sizeof(GPUDotData_t), this->dots);

                    glDrawArraysInstanced(GL_POINTS, 0, 1, this->dot_counter);

                    // std::cout << "DOT" << std::endl;

                    glPointSize(1.0f);
                    shader = 0xFFFF;
                    break;
                }
                break;
            }
        }
    }

    this->cmd_counter = 0;
    this->inst_counter = 0;
    this->line_counter = 0;
    this->dot_counter = 0;
}

void Renderer::terminate(void) {}

// protected

void Renderer::init_preview_cube(void) {
    struct Mesh mesh = {0};
    mesh.counter = 36;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ibo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(v3), PREVIEW_CUBE_VERTICES, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(u32), PREVIEW_CUBE_INDICES, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v3), (void*) 0);
    glEnableVertexAttribArray(0);

    this->meshes[0] = mesh;
}

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