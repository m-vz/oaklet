#version 330 core

layout(location = 0) in vec3 modelspaceVertexPosition;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(modelspaceVertexPosition, 1);
}