#version 330 core

const int MAX_DIRECTIONAL_LIGHTS = 1;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

struct Attenuation {
    float exponential;
    float linear;
    float constant;
};

struct DirectionalLight {
    mat4 vp;
    sampler2D shadowMapTextureSampler;
    vec3 direction;
    vec3 color;
    float power;
};

struct SpotLight {
    mat4 vp;
    sampler2D shadowMapTextureSampler;
    vec3 position;
    vec3 direction;
    vec3 color;
    float power;
    float cutoff;
    Attenuation attenuation;
};

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int directionalLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform mat4 mvp, model;

layout(location = 0) in vec3 modelspaceVertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 modelspaceVertexNormal;
layout(location = 3) in vec3 modelspaceVertexTangent;
layout(location = 4) in vec4 color;

out VertexOut {
    vec2 tangentspaceVertexUV;
    vec4 vertexColor;
    vec3 worldspaceVertexPosition;
    mat3 tbn;
    vec4 directionalLightVertexPositions[MAX_DIRECTIONAL_LIGHTS];
    vec4 spotLightVertexPositions[MAX_SPOT_LIGHTS];
} vertexOut;

void main() {
    gl_Position = mvp * vec4(modelspaceVertexPosition, 1);

    vertexOut.tangentspaceVertexUV = vertexUV;
    vertexOut.vertexColor = color;

    vertexOut.worldspaceVertexPosition = (model * vec4(modelspaceVertexPosition, 1)).xyz;

    mat3 model3 = mat3(model);
    vec3 worldspaceVertexNormal = model3 * modelspaceVertexNormal;
    vec3 worldspaceVertexTangent = model3 * modelspaceVertexTangent;
    vec3 worldspaceVertexBitangent = model3 * cross(modelspaceVertexNormal, modelspaceVertexTangent);
    vertexOut.tbn = mat3(worldspaceVertexTangent, worldspaceVertexBitangent, worldspaceVertexNormal);

    for(int i = 0; i < directionalLightCount; i++)
        vertexOut.directionalLightVertexPositions[i] = directionalLights[i].vp * vec4(vertexOut.worldspaceVertexPosition, 1);
    for(int i = 0; i < spotLightCount; i++)
        vertexOut.spotLightVertexPositions[i] = spotLights[i].vp * vec4(vertexOut.worldspaceVertexPosition, 1);
}