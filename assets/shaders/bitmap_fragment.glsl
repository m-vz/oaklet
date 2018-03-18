#version 330 core

in vec2 uv;

uniform sampler2D textureSampler;
uniform vec3 textColor;

out vec4 color;

void main() {
    vec3 bitmap = texture(textureSampler, uv).rgb;
    float alpha = bitmap.r + bitmap.g + bitmap.b;
    color = vec4(textColor, 1) * vec4(bitmap, alpha);
}