#version 330 core

layout(location = 0) in vec3 modelspaceVertexPosition;
layout(location = 2) in vec3 modelspaceVertexNormal;
layout(location = 4) in vec4 color;

uniform mat4 model, view, projection;

out vec3 worldspaceNormal, worldspaceVertexPosition, worldspaceCameraDirection;
out vec4 vertexColor;

void main() {
    gl_Position = (projection * view * model) * vec4(modelspaceVertexPosition, 1);

    vertexColor = color;

    worldspaceVertexPosition = (model * vec4(modelspaceVertexPosition, 1)).xyz;
    worldspaceCameraDirection = (inverse(view) * (vec4(0, 0, 0, 1) - (view * vec4(worldspaceVertexPosition, 1)))).xyz; // TODO: This should be simplified.
    worldspaceNormal = (model * vec4(modelspaceVertexNormal, 1)).rgb;
}