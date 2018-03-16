#version 330 core

layout(location = 0) in vec3 modelspaceVertexPosition;
layout(location = 1) in vec2 vertexUV;

uniform mat4 model, view, projection;
uniform vec3 worldspaceLightDirection;

out vec2 uv;
out vec3 worldspaceVertexPosition, cameraspaceLightDirection, cameraspaceCameraDirection;

void main() {
    gl_Position = (projection * view * model) * vec4(modelspaceVertexPosition, 1);

    uv = vertexUV;
    worldspaceVertexPosition = (model * vec4(modelspaceVertexPosition, 1)).xyz;
    cameraspaceCameraDirection = vec3(0, 0, 0) - (view * model * vec4(modelspaceVertexPosition, 1)).xyz;
    cameraspaceLightDirection = (view * vec4(worldspaceLightDirection, 1)).xyz + cameraspaceCameraDirection;
}