#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

uniform vec2 windowSize;

out vec2 uv;

void main() {
    vec2 halfWindowSize = windowSize/2;
    gl_Position =  vec4((vertexPosition - halfWindowSize)/halfWindowSize, 0, 1);

    uv = vertexUV;
}