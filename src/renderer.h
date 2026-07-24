#ifndef ARM_RENDERER_H
#define ARM_RENDERER_H

#include "common.h"
#include "math.h"
#include "shader.h"

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
    std::string read(std::string &path); // ?
    TextureStatus bind(void);
    TextureStatus destroy(void);
};

struct Mesh {
    u32 vao, vbo, ibo;
    u32 counter; // mv to size?
};

struct Material {
    shID id;
    Vec4_t color;
    f32 roughness, metallic;
    u16 albedo; // ?
};

typedef struct {
    Mat4_t transform;
    Vec4_t tint;
} GPUInstanceData_t;

enum class RendererStatus : u8 {
    SUCCESS = 0,

};

enum class RendererPass : u8 {
    OPAQUE = 0,
    TRANSPARENT,
    UI
};

// enum class RendererCommandType : u8 {
//     SPRITE = 0,
//     TEXT,
//     MESH,
//     LINE,
//     PRIMITIVE,
//     GRID
// };

typedef struct {
    u64 key; // sort key

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
    void read_meshes(const char *path);
    void read_materials(const char *path);
    void push_cmd(meID mesh, maID material, Mat4_t transform, Vec4_t tint);
    // void push_cmd(const char *mesh, ..) look up table [str => meID?]?
    void draw(void);
    void terminate(void);
private:
    static constexpr u32 MAX_SHADERS = 16;
    static constexpr u32 MAX_TEXTURES = 64;
    static constexpr u32 MAX_MESHES = 64;
    static constexpr u32 MAX_MATERIALS = 128;
    static constexpr u32 MAX_INSTANCES = 2048;

    Shader shaders[MAX_SHADERS];
    Texture textures[MAX_TEXTURES];

    struct Mesh meshes[MAX_MESHES];
    struct Material materials[MAX_MATERIALS];

    RendererCommand_t commands[MAX_INSTANCES];
    u32 cmd_counter = 0;
    GPUInstanceData_t instances[MAX_INSTANCES];
    u32 inst_counter = 0;

    u32 ssbo;
};

}

#endif // !ARM_RENDERER_H