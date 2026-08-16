#version 410 core

in vec3 b_Normal;
in vec4 b_Tint;

out vec4 o_Color;

void main() {
    vec3 c_DirectionalLight = normalize(vec3(0.5f, 1.0f, 0.3f));
    vec3 c_Normal = normalize(b_Normal);

    o_Color = vec4(b_Tint.rgb * max(dot(c_Normal, c_DirectionalLight), 0.2f), b_Tint.a);
}