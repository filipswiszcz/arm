#version 410 core

in vec3 b_Position;

out vec4 o_Color;

void main() {
    vec2 c_Coord = b_Position.xz / 1.0f;
    vec2 c_Grid = abs(fract(c_Coord - 0.5f) - 0.5f) / fwidth(c_Coord);
    
    vec4 c_Color = vec4(0.5f, 0.5f, 0.5f, 1.0f - min(min(c_Grid.x, c_Grid.y), 1.0f));
    c_Color.a *= max(0.0f, 1.0f - (length(b_Position.xz) / 100.0f));
    // c_Color.a = max(c_Color.a, 0.1f);

    o_Color = c_Color;
}