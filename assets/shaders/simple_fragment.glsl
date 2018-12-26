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
    float far;
    samplerCube shadowMapTextureSampler;
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
uniform vec3 worldspaceCameraPosition;
uniform sampler2D diffuseTextureSampler, normalTextureSampler, specularTextureSampler;

in GeometryOut {
    vec2 tangentspaceVertexUV;
    vec4 vertexColor;
    vec3 worldspaceVertexPosition;
    mat3 tbn;
    vec4 directionalLightVertexPositions[MAX_DIRECTIONAL_LIGHTS];
    vec4 spotLightVertexPositions[MAX_SPOT_LIGHTS];
} geometryOut;

out vec3 color;

float shadowCalculation(vec4 lightVertexPosition, vec3 vertexNormal, vec3 lightDirection, sampler2D shadowMapTextureSampler) {
    float shadow = 0.0;
    vec3 position = (lightVertexPosition.xyz / lightVertexPosition.w) * 0.5 + 0.5; // perspective division (xyz/w) and transformation from [-1, 1] to [0, 1]
    float currentDepth = position.z;

    float bias = max(0.005 * (1.0 - dot(vertexNormal, lightDirection)), 0.0005);
    int offset = 1;

    if(position.z <= 1.0) { // leave position values outside the light view frustum at 0
        vec2 texelSize = 1.0/textureSize(shadowMapTextureSampler, 0);
        for(int y = -offset; y <= offset; y++)
            for(int x = -offset; x <= offset; x++)
                shadow += currentDepth - bias > texture(shadowMapTextureSampler, position.xy + vec2(x, y)*texelSize).r ? 1 : 0;
        shadow /= pow(2*offset + 1, 2); // divide by the number of samples
    }

    return shadow;
}

float shadowCalculation(vec3 vertexNormal, vec3 lightDirection, float far, samplerCube shadowMapTextureSampler) {
    float shadow = 0.0;

    float bias = max(0.08 * (1.0 - dot(vertexNormal, -lightDirection)), 0.008);
    float offset = 0.01;
    float samples = 2.0;

    float currentDepth = length(lightDirection) - bias;
    float step = offset/(samples*0.5);
    for(float z = -offset; z < offset; z += step)
        for(float y = -offset; y < offset; y += step)
            for(float x = -offset; x < offset; x += step)
                shadow += currentDepth > texture(shadowMapTextureSampler, -lightDirection + vec3(x, y, z)).r*far ? 1 : 0;
    shadow /= pow(samples, 3); // divide by the number of samples

    return shadow;
}

void main() {
    float shininess = 16;
    vec3 diffuseColor = clamp(texture(diffuseTextureSampler, geometryOut.tangentspaceVertexUV).rgb + geometryOut.vertexColor.rgb, 0, 1);
    vec3 ambientColor = vec3(0.05) * diffuseColor;
    vec3 specularColor = texture(specularTextureSampler, geometryOut.tangentspaceVertexUV).rgb;
    color = ambientColor;

    vec3 worldspaceNormal = normalize(geometryOut.tbn * normalize(texture(normalTextureSampler, geometryOut.tangentspaceVertexUV).rgb*2.0 - 1.0));
    vec3 worldspaceCameraDirection = normalize(worldspaceCameraPosition - geometryOut.worldspaceVertexPosition);

    for(int i = 0; i < directionalLightCount; i++) {
        float lambertian = max(dot(directionalLights[i].direction, worldspaceNormal), 0);
        float specular = 0;

        if(lambertian > 0) {
            vec3 halfDirection = normalize(directionalLights[i].direction + worldspaceCameraDirection);
            specular = pow(max(dot(halfDirection, worldspaceNormal), 0), shininess);
        }

        color += (diffuseColor*lambertian + specularColor*specular) * directionalLights[i].color*directionalLights[i].power *
                 (1 - shadowCalculation(geometryOut.directionalLightVertexPositions[i], worldspaceNormal, normalize(directionalLights[i].direction), directionalLights[i].shadowMapTextureSampler));
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
            vec3 halfDirection = normalize(worldspaceLightDirection + worldspaceCameraDirection);
            specular = pow(max(dot(halfDirection, worldspaceNormal), 0), shininess);
        }

//        vec3 lightDirection = pointLights[i].position - geometryOut.worldspaceVertexPosition;
//        float bias = max(0.08 * (1.0 - dot(worldspaceNormal, lightDirection)), 0.008);
//        float currentDepth = length(lightDirection) - bias;
//        float closestDepth = texture(pointLights[i].shadowMapTextureSampler, -lightDirection).r*pointLights[i].far;
//        float thickness = currentDepth - closestDepth;
//        vec3 sssColor = vec3(1, 0.4, 0.2);
//        float sss = 1 - thickness;
//        if(sss <= 0)
//            sss = 0;
//        if(sss > 1)
//            sss = 1;
//        sss = pow(sss, 3);

        color += (diffuseColor*lambertian + specularColor*specular) * pointLights[i].color*pointLights[i].power / attenuation *
                 (1 - shadowCalculation(worldspaceNormal, pointLights[i].position - geometryOut.worldspaceVertexPosition, pointLights[i].far, pointLights[i].shadowMapTextureSampler))/* +
                 sssColor*sss*/;
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
                vec3 halfDirection = normalize(worldspaceLightDirection + worldspaceCameraDirection);
                specular = pow(max(dot(halfDirection, worldspaceNormal), 0), shininess);
            }

            color += (diffuseColor*lambertian + specularColor*specular) * spotLights[i].color*spotLights[i].power / attenuation *
                     (1 - (1 - spotlightFactor) * 1/(1 - spotLights[i].cutoff)) *
                     (1 - shadowCalculation(geometryOut.spotLightVertexPositions[i], worldspaceNormal, worldspaceLightDirection, spotLights[i].shadowMapTextureSampler));
        }
    }
}