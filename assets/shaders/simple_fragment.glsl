#version 330 core

const int MAX_POINT_LIGHTS = 8;

in vec2 tangentspaceVertexUV;
in vec4 vertexColor;
in vec3 worldspaceVertexPosition, worldspaceCameraDirection;
in mat3 tbn;

struct Attenuation {
    float exponential;
    float linear;
    float constant;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float power;
    Attenuation attenuation;
};

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform sampler2D diffuseTextureSampler, normalTextureSampler, specularTextureSampler;

out vec3 color;

void main() {
    vec3 diffuseColor = clamp(texture(diffuseTextureSampler, tangentspaceVertexUV).rgb + vertexColor.rgb, 0, 1);
    vec3 ambientColor = vec3(0.1) * diffuseColor;
    vec3 specularColor = texture(specularTextureSampler, tangentspaceVertexUV).rgb;
    color = vec3(0, 0, 0);

    vec3 worldspaceNormal = tbn * normalize(texture(normalTextureSampler, tangentspaceVertexUV).rgb*2.0 - 1.0);

    for(int i = 0; i < pointLightCount; i++) {
        vec3 worldspaceLightDirection = normalize(pointLights[i].position - worldspaceVertexPosition);

        vec3 reflection = reflect(-worldspaceLightDirection, worldspaceNormal);
        float cosAlpha = clamp(dot(normalize(worldspaceCameraDirection), reflection), 0, 1);

        float cosTheta = clamp(dot(worldspaceNormal, worldspaceLightDirection), 0, 1);

        float distance = length(pointLights[i].position - worldspaceVertexPosition);
        float attenuation = pointLights[i].attenuation.constant +
                            pointLights[i].attenuation.linear*distance +
                            pointLights[i].attenuation.exponential*distance*distance;

        color += ambientColor +
                 diffuseColor * pointLights[i].color*pointLights[i].power * cosTheta/attenuation +
                 specularColor * pointLights[i].color*pointLights[i].power * pow(cosAlpha, 5.0)/attenuation;
    }
}