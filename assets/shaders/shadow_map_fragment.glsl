#version 330 core

in vec4 worldspaceVertexPosition;

uniform bool cube;
uniform vec3 worldspaceLightPosition;
uniform float far;

void main() {
    if(cube) {
        float lightDistance = length(worldspaceVertexPosition.xyz - worldspaceLightPosition);
        gl_FragDepth = lightDistance / far; // divide by far plane distance to map to range [0, 1]
    } else
        gl_FragDepth = gl_FragCoord.z;
}
