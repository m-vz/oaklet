#version 330 core

uniform mat4 vp;

layout(location = 0) in vec3 vertexPosition;

out vec3 uv;

void main() {
    gl_Position = vp * vec4(vertexPosition, 1);

    uv = vertexPosition;
}