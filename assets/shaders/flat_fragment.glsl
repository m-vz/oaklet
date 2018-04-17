#version 330 core

const int MAX_POINT_LIGHTS = 8;

in vec3 worldspaceNormal, worldspaceVertexPosition, worldspaceCameraDirection;
in vec4 vertexColor;

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

out vec3 color;

void main() {
    vec3 diffuseColor = vertexColor.rgb;
    vec3 specularColor = vec3(1, 1, 1);
    color = vec3(0, 0, 0);

    for(int i = 0; i < pointLightCount; i++) {
        vec3 worldspaceLightDirection = normalize(pointLights[i].position - worldspaceVertexPosition);

        vec3 reflection = reflect(-worldspaceLightDirection, worldspaceNormal);
        float cosAlpha = clamp(dot(normalize(worldspaceCameraDirection), reflection), 0, 1);

        float cosTheta = clamp(dot(worldspaceNormal, worldspaceLightDirection), 0, 1);

        float distance = length(pointLights[i].position - worldspaceVertexPosition);
        float attenuation = pointLights[i].attenuation.constant +
                                pointLights[i].attenuation.linear*distance +
                                pointLights[i].attenuation.exponential*distance*distance;

        color += diffuseColor * pointLights[i].color*pointLights[i].power * cosTheta/attenuation +
                 specularColor * pointLights[i].color*pointLights[i].power * pow(cosAlpha, 5.0)/attenuation;
    }
}