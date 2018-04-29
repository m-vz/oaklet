#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform bool cube;
uniform mat4 vps[6];

out vec4 worldspaceVertexPosition;

void main() {
    int faceCount = cube ? 6 : 1;
    for(int face = 0; face < faceCount; face++) { // for each face on the depth map cube (or just once)
        gl_Layer = face; // specify the cube map face to render
        for(int i = 0; i < 3; i++) { // for each vertex
            worldspaceVertexPosition = gl_in[i].gl_Position;
            gl_Position = vps[face] * worldspaceVertexPosition;
            EmitVertex();
        }
        EndPrimitive();
    }
}
