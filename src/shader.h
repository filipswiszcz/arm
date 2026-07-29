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
    ShaderStatus initialize(std::string paths[2]);
    ShaderStatus use(void);
    i32 get_program() const {return program;}
    ShaderStatus set_uint(std::string name, u32 val);
    void set_vec3(std::string name, v3 vec);
    void set_mat4(std::string name, m4 mat);
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