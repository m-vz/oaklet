#version 330 core

uniform sampler2D renderedTextureSampler;

in vec2 uv;

out vec4 color;

void main() {
    color = texture(renderedTextureSampler, uv);

    // gamma correction (conversion from linear colours to srgb)
    if(color.r <= 0.00313066844250063) color.r *= 12.92;
    else color.r = 1.055*pow(color.r, 0.416666667) - 0.055;
    if(color.g <= 0.00313066844250063) color.g *= 12.92;
    else color.g = 1.055*pow(color.g, 0.416666667) - 0.055;
    if(color.b <= 0.00313066844250063) color.b *= 12.92;
    else color.b = 1.055*pow(color.b, 0.416666667) - 0.055;
}
