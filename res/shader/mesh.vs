#version 410 core

layout (location = 0) in vec3 l_Position;
layout (location = 1) in vec3 l_Normal;
// layout (location = 2) in vec2 l_Uv;

layout (std140) uniform u_Camera {
    mat4 u_ViewProjection;
    vec3 u_Position;
};

struct GPUInstanceData {
    mat4 transform;
    vec4 tint;
};

layout (std140) uniform u_Instances {
    GPUInstanceData instances[512];
};

uniform uint u_Index;

out vec3 b_Normal;
out vec4 b_Tint;

void main() {
    mat4 c_Model = instances[u_Index].transform;

    b_Normal = mat3(transpose(inverse(c_Model))) * l_Normal;
    b_Tint = instances[u_Index].tint;

    gl_Position = u_ViewProjection * c_Model * vec4(l_Position, 1.0f);
}