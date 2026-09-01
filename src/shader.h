#ifndef __SHADER_H__
#define __SHADER_H__

#include <fstream>
#include <string>

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #define GLFW_INCLUDE_GLCOREARB
    #include <GLFW/glfw3.h>
#else
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif

#include "common.h"
#include "math.h" // temp

namespace Renderer {

enum class ShaderStatus : u8 {
    SUCCESS = 0,
    FILE_NOT_FOUND,
    COMPILATION_FAILED,
    LINK_FAILED,
    CREATION_FAILED,
    INVALID_PARAMETER
};

class Shader {
public:
    Shader() : program(0) {
        ids[0] = 0;
        ids[1] = 0;
    }
    ShaderStatus initialize(std::string paths[2]); // dont use string you silly goose
    ShaderStatus use(void);
    i32 get() const {return program;} // add asserts
    ShaderStatus set(std::string name, f32 val);
    ShaderStatus set(std::string name, f64 val);
    ShaderStatus set(std::string name, i8 val);
    ShaderStatus set(std::string name, i16 val);
    ShaderStatus set(std::string name, i32 val);
    ShaderStatus set(std::string name, i64 val);
    ShaderStatus set(std::string name, u8 val);
    ShaderStatus set(std::string name, u16 val);
    ShaderStatus set(std::string name, u32 val);
    ShaderStatus set(std::string name, u64 val);
    ShaderStatus set(std::string name, v2 vec);
    ShaderStatus set(std::string name, v3 vec);
    ShaderStatus set(std::string name, v4 vec);
    // ShaderStatus set(std::string name, m2 mat);
    // ShaderStatus set(std::string name, m3 mat);
    ShaderStatus set(std::string name, m4 mat);
private:
    i32 ids[2]; // [0] = vert, [1] = frag
    i32 program;
    std::string read(std::string &path);
    // ShaderStatus read(std::string &path);
    ShaderStatus compile(i32 &id, u32 type, const std::string &code); // i dont need args here probably
    ShaderStatus link(void);
    // ShaderStatus delete();
};

}

#endif // !__SHADER_H__