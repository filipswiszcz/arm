#version 410 core

layout (location = 0) in vec3 l_Pos;
layout (location = 1) in vec4 l_Color;
layout (location = 2) in float l_Thickness;

layout (std140) uniform u_Camera {
    mat4 u_ViewProjection;
    vec3 u_Position;
};

out vec4 b_Color;

void main() {
    b_Color = l_Color;
    gl_PointSize = l_Thickness;
    gl_Position = u_ViewProjection * vec4(l_Pos, 1.0f);
}