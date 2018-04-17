#version 330 core

layout(location = 0) in vec3 modelspaceVertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 modelspaceVertexNormal;
layout(location = 3) in vec3 modelspaceVertexTangent;
layout(location = 4) in vec4 color;

uniform mat4 mvp, model, view;

out vec2 tangentspaceVertexUV;
out vec4 vertexColor;
out vec3 worldspaceVertexPosition, worldspaceCameraDirection;
out mat3 tbn;

void main() {
    gl_Position = mvp * vec4(modelspaceVertexPosition, 1);

    tangentspaceVertexUV = vertexUV;
    vertexColor = color;

    worldspaceVertexPosition = (model * vec4(modelspaceVertexPosition, 1)).xyz;
    worldspaceCameraDirection = (inverse(view) * (vec4(0, 0, 0, 1) - (view * vec4(worldspaceVertexPosition, 1)))).xyz; // TODO: This should be simplified.

    mat3 model3 = mat3(model);
    vec3 worldspaceVertexNormal = model3 * modelspaceVertexNormal;
    vec3 worldspaceVertexTangent = model3 * modelspaceVertexTangent;
    vec3 worldspaceVertexBitangent = model3 * cross(modelspaceVertexNormal, modelspaceVertexTangent);
    tbn = mat3(worldspaceVertexTangent, worldspaceVertexBitangent, worldspaceVertexNormal);
}