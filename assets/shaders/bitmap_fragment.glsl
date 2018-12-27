#version 330 core

uniform sampler2D bitmapTextureSampler;

in VertexOut {
    vec2 uv;
    vec3 vertexColor;
} vertexOut;

out vec4 color;

void main() {
    vec3 bitmap = texture(bitmapTextureSampler, vertexOut.uv).rgb;
    float alpha = bitmap.r + bitmap.g + bitmap.b;
    color = vec4(vertexOut.vertexColor, 1) * vec4(bitmap, alpha);
}