#version 330 core

const int MAX_DIRECTIONAL_LIGHTS = 1;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexOut {
    vec2 tangentspaceVertexUV;
    vec4 vertexColor;
    vec3 worldspaceVertexPosition, worldspaceCameraDirection;
    mat3 tbn;
    vec4 directionalLightVertexPositions[MAX_DIRECTIONAL_LIGHTS];
    vec4 spotLightVertexPositions[MAX_SPOT_LIGHTS];
} vertexOut[];

out GeometryOut {
    vec2 tangentspaceVertexUV;
    vec4 vertexColor;
    vec3 worldspaceVertexPosition, worldspaceCameraDirection;
    mat3 tbn;
    vec4 directionalLightVertexPositions[MAX_DIRECTIONAL_LIGHTS];
    vec4 spotLightVertexPositions[MAX_SPOT_LIGHTS];
} geometryOut;

void main() {
    for(int i = 0; i < gl_in.length(); i++) {
        geometryOut.tangentspaceVertexUV = vertexOut[i].tangentspaceVertexUV;
        geometryOut.vertexColor = vertexOut[i].vertexColor;
        geometryOut.worldspaceVertexPosition = vertexOut[i].worldspaceVertexPosition;
        geometryOut.worldspaceCameraDirection = vertexOut[i].worldspaceCameraDirection;
        geometryOut.tbn = vertexOut[i].tbn;
        geometryOut.directionalLightVertexPositions = vertexOut[i].directionalLightVertexPositions;
        geometryOut.spotLightVertexPositions = vertexOut[i].spotLightVertexPositions;

        gl_Position =  gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
