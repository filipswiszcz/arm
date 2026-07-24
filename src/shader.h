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
    CREATION_FAILED
};

class Shader {
public:
    Shader() : program(0) {
        ids[0] = 0;
        ids[1] = 0;
    }
    i32 get_program() const {return program;}
    ShaderStatus initialize(std::string paths[2]);
    void set_vec3(std::string name, Vec3_t vec);
    void set_mat4(std::string name, Mat4_t mat);
private:
    i32 ids[2]; // [0] = vert, [1] = frag
    i32 program;
    std::string read(std::string &path);
    // ShaderStatus read(std::string &path);
    ShaderStatus compile(i32 &id, u32 type, const std::string &code);
    ShaderStatus link();
    // ShaderStatus delete();
};

}

#endif // !__SHADER_H__