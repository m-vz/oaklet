#version 330 core

const int MAX_DIRECTIONAL_LIGHTS = 1;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;
const float SCREEN_GAMMA = 2.2;

in vec2 tangentspaceVertexUV;
in vec4 vertexColor;
in vec3 worldspaceVertexPosition, worldspaceCameraDirection;
in mat3 tbn;

struct Attenuation {
    float exponential;
    float linear;
    float constant;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float power;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float power;
    Attenuation attenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float power;
    float cutoff;
    Attenuation attenuation;
};

uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int directionalLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform sampler2D diffuseTextureSampler, normalTextureSampler, specularTextureSampler;

out vec3 color;

void main() {
    float shininess = 16;
    vec3 diffuseColor = clamp(texture(diffuseTextureSampler, tangentspaceVertexUV).rgb + vertexColor.rgb, 0, 1);
    vec3 ambientColor = vec3(0.2) * diffuseColor;
    vec3 specularColor = texture(specularTextureSampler, tangentspaceVertexUV).rgb;
    color = ambientColor;

    vec3 worldspaceNormal = tbn * normalize(texture(normalTextureSampler, tangentspaceVertexUV).rgb*2.0 - 1.0);

    for(int i = 0; i < directionalLightCount; i++) {
        float lambertian = max(dot(directionalLights[i].direction, worldspaceNormal), 0);
        float specular = 0;

        if(lambertian > 0) {
            vec3 halfDirection = normalize(directionalLights[i].direction + worldspaceCameraDirection);
            float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
            specular = pow(cosAlpha, shininess);
        }

        color += diffuseColor * directionalLights[i].color*directionalLights[i].power * lambertian +
                 specularColor * directionalLights[i].color*directionalLights[i].power * specular;
    }

    for(int i = 0; i < pointLightCount; i++) {
        vec3 worldspaceLightDirection = pointLights[i].position - worldspaceVertexPosition;
        float distance = length(worldspaceLightDirection);
        float attenuation = pointLights[i].attenuation.constant +
                            pointLights[i].attenuation.linear*distance +
                            pointLights[i].attenuation.exponential*distance*distance;
        worldspaceLightDirection = normalize(worldspaceLightDirection);

        float lambertian = max(dot(worldspaceLightDirection, worldspaceNormal), 0);
        float specular = 0;

        if(lambertian > 0) {
            vec3 halfDirection = normalize(worldspaceLightDirection + worldspaceCameraDirection);
            float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
            specular = pow(cosAlpha, shininess);
        }

        color += diffuseColor * pointLights[i].color*pointLights[i].power * lambertian / attenuation +
                 specularColor * pointLights[i].color*pointLights[i].power * specular / attenuation;
    }

    for(int i = 0; i < spotLightCount; i++) {
        vec3 worldspaceLightDirection = spotLights[i].position - worldspaceVertexPosition;
        float distance = length(worldspaceLightDirection);
        float attenuation = spotLights[i].attenuation.constant +
                            spotLights[i].attenuation.linear*distance +
                            spotLights[i].attenuation.exponential*distance*distance;
        worldspaceLightDirection = normalize(worldspaceLightDirection);

        float spotlightFactor = dot(worldspaceLightDirection, -spotLights[i].direction);

        if(spotlightFactor > spotLights[i].cutoff) {
            float lambertian = max(dot(worldspaceLightDirection, worldspaceNormal), 0);
            float specular = 0;

            if(lambertian > 0) {
                vec3 halfDirection = normalize(worldspaceLightDirection + worldspaceCameraDirection);
                float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
                specular = pow(cosAlpha, shininess);
            }

            color += (diffuseColor * spotLights[i].color*spotLights[i].power * lambertian / attenuation +
                     specularColor * spotLights[i].color*spotLights[i].power * specular / attenuation) *
                     (1 - (1 - spotlightFactor) * 1/(1 - spotLights[i].cutoff));
        }
    }

    // gamma correction
    // color = pow(color, vec3(1/SCREEN_GAMMA));
}