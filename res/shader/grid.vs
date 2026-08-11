#version 410 core

layout (std140) uniform u_Camera {
    mat4 u_ViewProj;
    vec3 u_Position;
};

out vec3 b_Position;

const vec3 VERTICES[6] = vec3[6](
    vec3(-1.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 1.0f),
    vec3(1.0f, 0.0f, -1.0f), vec3(1.0f, 0.0f, 1.0f), vec3(-1.0f, 0.0f, 1.0f)
);

void main() {
    vec3 c_Position = VERTICES[gl_VertexID] * 512.0f;
    c_Position.x += u_Position.x;
    c_Position.z += u_Position.z;

    b_Position = c_Position;
    gl_Position = u_ViewProj * vec4(c_Position, 1.0f);
}