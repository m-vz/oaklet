#version 330 core

in vec3 worldspaceNormal, worldspaceVertexPosition, worldspaceCameraDirection;
in vec4 vertexColor;

uniform vec3 worldspaceLightPosition;

out vec3 color;

void main() {
    vec3 lightColor = vec3(1, 1, 1);
    float lightPower = 1.0f;

    vec3 diffuseColor = vertexColor.rgb;
    vec3 ambientColor = vec3(0.1, 0.1, 0.1) * diffuseColor;
    vec3 specularColor = vec3(1, 1, 1);

    vec3 worldspaceLightDirection = normalize(worldspaceLightPosition - worldspaceVertexPosition);

    vec3 reflection = reflect(-worldspaceLightDirection, worldspaceNormal);
    float cosAlpha = clamp(dot(normalize(worldspaceCameraDirection), reflection), 0, 1);

    float cosTheta = clamp(dot(worldspaceNormal, worldspaceLightDirection), 0, 1);

    float distanceSquared = pow(length(worldspaceLightPosition - worldspaceVertexPosition), 2.0);

    color = ambientColor +
            diffuseColor * lightColor*lightPower * cosTheta/distanceSquared +
            specularColor * lightColor*lightPower * pow(cosAlpha, 5.0)/distanceSquared;
}