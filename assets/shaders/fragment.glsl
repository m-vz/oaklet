#version 330 core

in vec2 uv;
in vec3 worldspaceVertexPosition, cameraspaceLightDirection, cameraspaceCameraDirection;

uniform sampler2D medievalHouseDiffuseTextureSampler, medievalHouseNormalTextureSampler, medievalHouseSpecularTextureSampler;
uniform vec3 worldspaceLightDirection;

out vec3 color;

void main() {
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 50000.0f;

    vec3 diffuseColor = texture(medievalHouseDiffuseTextureSampler, uv).rgb;
    vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
    vec3 specularColor = texture(medievalHouseSpecularTextureSampler, uv).rgb;

    vec3 cameraspaceLightDirectionNormalized = normalize(cameraspaceLightDirection);
    vec3 normal = normalize(texture(medievalHouseNormalTextureSampler, uv).rgb);
    vec3 reflection = reflect(-cameraspaceLightDirectionNormalized, normal);
    float cosAlpha = clamp(dot(normalize(cameraspaceCameraDirection), reflection), 0, 1);
    float cosTheta = clamp(dot(normal, cameraspaceLightDirectionNormalized), 0, 1);
    float distance = length(worldspaceLightDirection - worldspaceVertexPosition);

    color = ambientColor +
            diffuseColor * lightColor*lightPower * cosTheta/(distance*distance) +
            specularColor * lightColor*lightPower * pow(cosAlpha, 5)/(distance*distance);
}