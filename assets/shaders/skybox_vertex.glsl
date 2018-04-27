#version 330 core

uniform mat4 vp;

layout(location = 0) in vec3 vertexPosition;

out vec3 uv;

void main() {
    vec4 pos = vp * vec4(vertexPosition, 1);
    gl_Position = pos.xyww;

    uv = vertexPosition;
}