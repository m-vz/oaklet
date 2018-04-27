#version 330 core

const int MAX_DIRECTIONAL_LIGHTS = 1;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;
const float SCREEN_GAMMA = 2.2;

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

struct PointLight {
    vec3 position;
    vec3 color;
    float power;
    Attenuation attenuation;
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
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform sampler2D diffuseTextureSampler, normalTextureSampler, specularTextureSampler;

in GeometryOut {
    vec2 tangentspaceVertexUV;
    vec4 vertexColor;
    vec3 worldspaceVertexPosition, worldspaceCameraDirection;
    mat3 tbn;
    vec4 directionalLightVertexPositions[MAX_DIRECTIONAL_LIGHTS];
    vec4 spotLightVertexPositions[MAX_SPOT_LIGHTS];
} geometryOut;

out vec3 color;

float shadowCalculation(vec4 lightVertexPosition, vec3 vertexNormal, vec3 lightDirection, sampler2D shadowMapTextureSampler) {
    vec3 position = (lightVertexPosition.xyz / lightVertexPosition.w) * 0.5 + 0.5; // perspective division (xyz/w) and transformation from [-1, 1] to [0, 1]
    float currentDepth = position.z;
    float bias = max(0.005 * (1.0 - dot(vertexNormal, lightDirection)), 0.0005);

    float shadow = 0.0;

    if(position.z <= 1.0) { // leave position values outside the light view frustum at 0
        vec2 texelSize = 1.0/textureSize(shadowMapTextureSampler, 0);
        for(int y = -1; y <= 1; y++)
            for(int x = -1; x <= 1; x++)
                shadow += currentDepth - bias > texture(shadowMapTextureSampler, position.xy + vec2(x, y)*texelSize).r ? 1 : 0;
        shadow /= 9;
    }

    return shadow;
}

void main() {
    float shininess = 16;
    vec3 diffuseColor = clamp(texture(diffuseTextureSampler, geometryOut.tangentspaceVertexUV).rgb + geometryOut.vertexColor.rgb, 0, 1);
    vec3 ambientColor = vec3(0.05) * diffuseColor;
    vec3 specularColor = texture(specularTextureSampler, geometryOut.tangentspaceVertexUV).rgb;
    color = ambientColor;

    vec3 worldspaceNormal = geometryOut.tbn * normalize(texture(normalTextureSampler, geometryOut.tangentspaceVertexUV).rgb*2.0 - 1.0);

    for(int i = 0; i < directionalLightCount; i++) {
        float lambertian = max(dot(directionalLights[i].direction, worldspaceNormal), 0);
        float specular = 0;

        if(lambertian > 0) {
            vec3 halfDirection = normalize(directionalLights[i].direction + geometryOut.worldspaceCameraDirection);
            float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
            specular = pow(cosAlpha, shininess);
        }

        color += (diffuseColor * directionalLights[i].color*directionalLights[i].power * lambertian +
                 specularColor * directionalLights[i].color*directionalLights[i].power * specular) *
                 (1 - shadowCalculation(geometryOut.directionalLightVertexPositions[i], worldspaceNormal, normalize(spotLights[i].position - geometryOut.worldspaceVertexPosition), directionalLights[i].shadowMapTextureSampler));
    }

    for(int i = 0; i < pointLightCount; i++) {
        vec3 worldspaceLightDirection = pointLights[i].position - geometryOut.worldspaceVertexPosition;
        float distance = length(worldspaceLightDirection);
        float attenuation = pointLights[i].attenuation.constant +
                            pointLights[i].attenuation.linear*distance +
                            pointLights[i].attenuation.exponential*distance*distance;
        worldspaceLightDirection = normalize(worldspaceLightDirection);

        float lambertian = max(dot(worldspaceLightDirection, worldspaceNormal), 0);
        float specular = 0;

        if(lambertian > 0) {
            vec3 halfDirection = normalize(worldspaceLightDirection + geometryOut.worldspaceCameraDirection);
            float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
            specular = pow(cosAlpha, shininess);
        }

        color += diffuseColor * pointLights[i].color*pointLights[i].power * lambertian / attenuation +
                 specularColor * pointLights[i].color*pointLights[i].power * specular / attenuation;
    }

    for(int i = 0; i < spotLightCount; i++) {
        vec3 worldspaceLightDirection = spotLights[i].position - geometryOut.worldspaceVertexPosition;
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
                vec3 halfDirection = normalize(worldspaceLightDirection + geometryOut.worldspaceCameraDirection);
                float cosAlpha = max(dot(halfDirection, worldspaceNormal), 0);
                specular = pow(cosAlpha, shininess);
            }

            color += (diffuseColor * spotLights[i].color*spotLights[i].power * lambertian / attenuation +
                     specularColor * spotLights[i].color*spotLights[i].power * specular / attenuation) *
                     (1 - (1 - spotlightFactor) * 1/(1 - spotLights[i].cutoff)) *
                     (1 - shadowCalculation(geometryOut.spotLightVertexPositions[i], worldspaceNormal, worldspaceLightDirection, spotLights[i].shadowMapTextureSampler));
        }
    }

    // gamma correction (conversion from linear colours to srgb)
    if(color.r <= 0.00313066844250063) color.r *= 12.92;
    else color.r = 1.055*pow(color.r, 0.416666667) - 0.055;
    if(color.g <= 0.00313066844250063) color.g *= 12.92;
    else color.g = 1.055*pow(color.g, 0.416666667) - 0.055;
    if(color.b <= 0.00313066844250063) color.b *= 12.92;
    else color.b = 1.055*pow(color.b, 0.416666667) - 0.055;
}