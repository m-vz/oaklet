#version 330 core

uniform vec2 windowSize;

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 color;

out VertexOut {
    vec2 uv;
    vec3 vertexColor;
} vertexOut;

void main() {
    vec2 halfWindowSize = windowSize/2;
    gl_Position =  vec4(vertexPosition.x/halfWindowSize.x - 1, 1 - vertexPosition.y/halfWindowSize.y, 0, 1);

    vertexOut.uv = vertexUV;
    vertexOut.vertexColor = color;
}