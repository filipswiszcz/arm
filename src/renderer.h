#ifndef ARM_RENDERER_H
#define ARM_RENDERER_H

#include <filesystem>

#include "common.h"
#include "math.h"
#include "shader.h"

#define DEBUG_RENDERER_MESH_SHADER_ID 0
#define DEBUG_RENDERER_GRID_SHADER_ID 1
#define DEBUG_RENDERER_LINE_SHADER_ID 2

namespace Renderer {

typedef u16 shID; // shader
typedef u16 maID; // material
typedef u16 meID; // mesh

enum class TextureStatus : u8 {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    COMPILATION_FAILED,
    LINK_FAILED,
    CREATION_FAILED
};

class Texture {
public:
    Texture() : id(0), width(0), height(0), format(0), channels(0) {}
    TextureStatus initialize(std::string path);
private:
    u32 id;
    i32 width, height;
    i32 format, channels;
    std::string read(const char *path);
    TextureStatus bind(void);
    TextureStatus destroy(void);
};

struct Mesh {
    u32 vao, vbo, ibo;
    u32 counter; // mv to size?
};

struct Material {
    shID id;
    v4 color;
    f32 roughness, metallic;
    u16 albedo; // ?
};

typedef struct {
    m4 transform;
    v4 tint;
} GPUInstanceData_t;

typedef struct {
    m4 view_proj;
    v3 pos; // with padding (std140)
    f32 padd; // can i just use v4?
} GPUCameraData_t;

typedef struct {
    v3 start, end;
    v4 color;
    f32 thickness;
} GPULineData_t;

enum class RendererStatus : u8 {
    SUCCESS = 0,
    FILE_NOT_FOUND
};

enum class RendererPass : u8 {
    SHADOW = 0,
    DEPTH_PREPASS,
    OPAQUE,
    SKYBOX,
    ALPHA_TEST,
    TRANSPARENT,
    PARTICLES,
    POST_PROCESS,
    UI,
    DEBUG
};

/*enum class RendererCommandType : u8 {
    SPRITE = 0,
    TEXT,
    MESH,
    LINE,
    PRIMITIVE,
    GRID
};*/

enum class RendererCommandType : u8 {
    GRID = 0,
    MESH,
    TEXT,
    LINE,
    DOT
};

typedef struct {
    u64 key; // sort key

    RendererCommandType type;

    meID mesh;
    maID material;

    u32 index;
} RendererCommand_t;

/*typedef struct {
    union {

        struct {
            Texture *texture;
            Vec2_t scale, offset;
        } sprite;
        
        struct {
            char content[64]; // change to smth else (string from arena?)
            f32 scale;
        } text;

        struct {
            // mesh
            // material
        } mesh;

        struct {
            Vec3_t start, end;
            f32 thickness;
        } line;

        struct {} primitive;

        struct {} grid;
    
    } data;

    RendererCommandType type;

    Vec2_t pos, size;
    f32 rot;

    Vec3_t color;
} RendererCommand_t;*/

class Renderer {
public:
    void initialize(void);
    void read_shaders(const char *path);
    void read_textures(const char *path);
    void read_meshes(const char *path);
    void read_materials(const char *path);
    void push_cmd(meID mesh, maID material, m4 transform, v4 tint);
    // void push_cmd(const char *mesh, ..) look up table [str => meID?]?
    void push_cmd(v3 start, v3 end, v4 color, f32 thickness);
    // void push_cmd(v3 pos, v4 color, f32 thickness);
    void push_cmd(const char *text, v2 pos, f32 scale, v4 color);
    void push_cmd(v4 color); // ? (grid)
    // void draw(void);
    void draw(m4 view_proj, v3 cam_pos); // temp solution
    void terminate(void);
// protected:
    void init_preview_cube(void);
private:
    static constexpr u32 MAX_SHADERS = 16;
    static constexpr u32 MAX_TEXTURES = 64;
    static constexpr u32 MAX_MESHES = 64;
    static constexpr u32 MAX_MATERIALS = 128;
    static constexpr u32 MAX_INSTANCES = 2048;
    static constexpr u32 MAX_LINES = 4096;
    static constexpr u32 CHUNK_SIZE = 512;

    Shader shaders[MAX_SHADERS];
    Texture textures[MAX_TEXTURES];

    struct Mesh meshes[MAX_MESHES];
    struct Material materials[MAX_MATERIALS];

    RendererCommand_t commands[MAX_INSTANCES];
    u32 cmd_counter = 0;
    GPUInstanceData_t instances[MAX_INSTANCES];
    u32 inst_counter = 0;
    GPULineData_t lines[MAX_LINES];
    u32 line_counter = 0;

    struct {
        struct {u32 vao;} grid;
        struct {u32 ubo;} mesh;
        struct {u32 ubo;} camera;
        struct {u32 vao, vbo;} text;
        struct {u32 vao, vbo;} line;
    } gpu;
};

}

#endif // !ARM_RENDERER_H