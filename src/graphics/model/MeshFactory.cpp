//
// Created by Milan van Zanten on 27.03.18.
//

#include "MeshFactory.h"

Model *MeshFactory::addPlane(Model *addTo, glm::vec3 origin, float width, float height, glm::vec3 normal, glm::vec3 up, glm::vec4 color) {
    auto *plane = new Mesh();

    /* create mesh */
    float widthHalf = width/2.0f, heightHalf = height/2.0f;
    up -= glm::dot(normal, up)/glm::dot(normal, normal) * normal;
    glm::vec3 tangent = glm::cross(up, normal);
    glm::vec3 vertices[4] = {
            origin - widthHalf*tangent - heightHalf*up,
            origin + widthHalf*tangent - heightHalf*up,
            origin + widthHalf*tangent + heightHalf*up,
            origin - widthHalf*tangent + heightHalf*up,
    };
    unsigned int indices[6] = {
            0, 1, 2,
            0, 2, 3
    };

    for(auto &vertex: vertices) {
        plane->vertexData.push_back(vertex.x);
        plane->vertexData.push_back(vertex.y);
        plane->vertexData.push_back(vertex.z);

        plane->normalData.push_back(normal.x);
        plane->normalData.push_back(normal.y);
        plane->normalData.push_back(normal.z);

        plane->tangentData.push_back(tangent.x);
        plane->tangentData.push_back(tangent.y);
        plane->tangentData.push_back(tangent.z);

        plane->colorData.push_back(color.r);
        plane->colorData.push_back(color.g);
        plane->colorData.push_back(color.b);
        plane->colorData.push_back(color.a);
    }

    for(auto &index: indices)
        plane->indices.push_back(index);

    /* fill mesh and add to model */
    plane->fillBuffers(false, true, true, true);

    addTo->meshes.push_back(plane);
    return addTo;
}
