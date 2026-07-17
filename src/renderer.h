#ifndef ARM_RENDERER_H
#define ARM_RENDERER_H

#include "common.h"
#include "math.h"
#include "shader.h"

namespace Renderer {

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
private:
    u32 id;
    i32 width, height;
    i32 format, channels;
    TextureStatus initialize(std::string &path);
    TextureStatus bind(void);
    TextureStatus destroy(void);
};

enum class RendererCommandType : u8 {
    SPRITE = 0,
    TEXT,
    MESH,
    LINE,
    PRIMITIVE,
    GRID
};

typedef struct {
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
} RendererCommand_t;

class Renderer {
public:
private:
};

}

#endif // !ARM_RENDERER_H