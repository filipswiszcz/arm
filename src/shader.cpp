#include "shader.h"

// public

namespace Renderer {

ShaderStatus Shader::initialize(std::string paths[2]) {
    std::string vs_code = this->read(paths[0]);
    std::string fs_code = this->read(paths[1]);

    // empty code asserts

    ShaderStatus vs_compile_status = this->compile(this->ids[0], GL_VERTEX_SHADER, vs_code);
    if (vs_compile_status != ShaderStatus::SUCCESS) {
        return vs_compile_status;
    }
    ShaderStatus fs_compile_status = this->compile(this->ids[1], GL_FRAGMENT_SHADER, fs_code);
    if (fs_compile_status != ShaderStatus::SUCCESS) {
        // delete vertex shader, because it went ok
        return fs_compile_status;
    }

    this->program = glCreateProgram();
    if (this->program == 0) {
        // delete shaders etc
        return ShaderStatus::CREATION_FAILED;
    }

    glAttachShader(this->program, this->ids[0]);
    glAttachShader(this->program, this->ids[1]);

    return this->link();
}

ShaderStatus Shader::use(void) {
    // some asserts
    glUseProgram(this->program);
    return ShaderStatus::SUCCESS;
}

ShaderStatus Shader::set_uint(std::string name, u32 val) {
    if (!ENGINE_ASSERT(name.c_str() != NULL)) {
        return ShaderStatus::INVALID_PARAMETER;
    }
    glUniform1ui(glGetUniformLocation(this->program, name.c_str()), val);
    return ShaderStatus::SUCCESS;
}

void Shader::set_vec3(std::string name, Vec3_t vec) {
    glUniform3f(glGetUniformLocation(this->program, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::set_mat4(std::string name, Mat4_t mat) {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, 0, &mat.m[0][0]);
}

// private

std::string Shader::read(std::string &path) {
    std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);

    // assert file open

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string code(size, '\0');

    // assert file read
    if (file.read(&code[0], size)) {
        return code;
    }

    return "";

    // return code;
}

ShaderStatus Shader::compile(i32 &id, u32 type, const std::string &code) {
    id = glCreateShader(type);
    if (id == 0) {
        return ShaderStatus::COMPILATION_FAILED;
    }

    const char *source = code.c_str();
    glShaderSource(id, 1, &source, nullptr);

    glCompileShader(id);

    i32 params;
    glGetShaderiv(id, GL_COMPILE_STATUS, &params);
    if (params == 0) {
        // print error in DEBUG
        // char log[512];
        // glGetShaderInfoLog(id, 512, NULL, log);
        // std::cout << "SHADER COMPILE ERROR: " << log << std::endl;

        glDeleteShader(id);
        id = 0;

        return ShaderStatus::COMPILATION_FAILED;
    }

    return ShaderStatus::SUCCESS;
}

ShaderStatus Shader::link() {
    glLinkProgram(this->program);

    i32 params;
    glGetProgramiv(this->program, GL_LINK_STATUS, &params);
    if (params == 0) {
        // print error in DEBUG
        return ShaderStatus::LINK_FAILED;
    }

    return ShaderStatus::SUCCESS;
}

}