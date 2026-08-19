#version 410 core

in vec3 b_Position;

out vec4 o_Color;

void main() {
    vec2 c_Coord = b_Position.xz;
    vec2 c_Grid = abs(fract(c_Coord - 0.5f) - 0.5f);
    vec2 c_Derivative = fwidth(c_Coord);

    float c_Alpha = max(
        (1.0f - smoothstep(0.0f, c_Derivative.x, c_Grid.x)), 
        (1.0f - smoothstep(0.0f, c_Derivative.y, c_Grid.y))
    );
    c_Alpha *= max(0.0f, 1.0f - length(c_Coord) / 100.0f);

    o_Color = vec4(0.5f, 0.5f, 0.5f, c_Alpha);
}