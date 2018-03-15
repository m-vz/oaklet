#version 330 core

layout(location = 0) in vec3 modelspaceVertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

uniform mat4 model, view, projection;

void main() {
    gl_Position = (projection * view * model) * vec4(modelspaceVertexPosition, 1);

    uv = vertexUV;
}