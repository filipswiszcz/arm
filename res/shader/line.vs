#version 410 core

layout (location = 0) in vec3 l_Start;
layout (location = 1) in vec3 l_End;
layout (location = 2) in vec4 l_Color;

layout (std140) uniform u_Camera {
    mat4 u_ViewProjection;
    vec3 u_Position;
};

out vec4 b_Color;

void main() {
    b_Color = l_Color;
    gl_Position = u_ViewProjection * vec4(((gl_VertexID == 0) ? l_Start : l_End), 1.0f);
}