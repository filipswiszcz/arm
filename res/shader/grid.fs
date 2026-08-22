#version 410 core

in vec3 b_Position;

out vec4 o_Color;

float gridalpha(vec2 c_Coord) {
    vec2 c_Distance = abs(fract(c_Coord - 0.5f) - 0.5f);
    vec2 c_Derivative = fwidth(c_Coord);
    return max(
        (1.0f - smoothstep(0.0f, c_Derivative.x, c_Distance.x)),
        (1.0f - smoothstep(0.0f, c_Derivative.y, c_Distance.y))
    );
}

void main() {
    vec2 c_Coord = b_Position.xz;

    float c_PrimaryAlpha = gridalpha(c_Coord) * 0.8f;
    float c_SecondaryAlpha = gridalpha(c_Coord * 10.0f) * (1.0f - c_PrimaryAlpha) * 0.2f;

    float c_Alpha = max(c_PrimaryAlpha, c_SecondaryAlpha);
    c_Alpha *= max(0.0f, 1.0f - length(c_Coord) / 100.0f);

    o_Color = vec4(0.5f, 0.5f, 0.5f, c_Alpha);
}