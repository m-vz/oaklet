//
// Created by Milan van Zanten on 27.03.18.
//

#include <array>
#include "MeshFactory.h"

Model *MeshFactory::addPlane(Model *addTo,
                             glm::vec3 origin,
                             float width, float height,
                             glm::vec3 normal, glm::vec3 up,
                             glm::vec4 color) {
    auto *plane = new Mesh();

    // normalize and orthogonalize
    normal = glm::normalize(normal);
    up = glm::normalize(up);
    up -= glm::dot(normal, up)/glm::dot(normal, normal) * normal;

    /* create mesh */
    float widthHalf = width/2.0f, heightHalf = height/2.0f;
    glm::vec3 tangent = glm::cross(up, normal);
    glm::vec3 vertices[4] = {
            origin - widthHalf*tangent - heightHalf*up,
            origin + widthHalf*tangent - heightHalf*up,
            origin + widthHalf*tangent + heightHalf*up,
            origin - widthHalf*tangent + heightHalf*up
    };
    glm::vec2 uvs[4] = {
            glm::vec2(0, 0),
            glm::vec2(1, 0),
            glm::vec2(1, 1),
            glm::vec2(0, 1)
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
    for(auto &uv: uvs) {
        plane->uvData.push_back(uv.x);
        plane->uvData.push_back(uv.y);
    }
    for(auto &index: indices)
        plane->indices.push_back(index);

    /* fill mesh and add to model */
    plane->fillBuffers(true, true, true, true);

    addTo->meshes.push_back(plane);
    return addTo;
}

Model *MeshFactory::addCuboid(Model *addTo,
                              glm::vec3 origin,
                              float width, float height, float length,
                              glm::vec3 forward, glm::vec3 up,
                              glm::vec4 color,
                              bool insideOut) {
    auto *cuboid = new Mesh();

    // normalize and orthogonalize
    forward = glm::normalize(forward);
    up = glm::normalize(up);
    up -= glm::dot(forward, up)/glm::dot(forward, forward) * forward;

    /* create mesh */
    float widthHalf = width/2.0f, heightHalf = height/2.0f, lengthHalf = length/2.0f;
    glm::vec3 left = glm::cross(up, forward), *tmp;
    glm::vec3 vertices[8] = {
            origin + widthHalf*left - heightHalf*up - lengthHalf*forward, // front bottom left
            origin - widthHalf*left - heightHalf*up - lengthHalf*forward, // front bottom right
            origin - widthHalf*left + heightHalf*up - lengthHalf*forward, // front top right
            origin + widthHalf*left + heightHalf*up - lengthHalf*forward, // front top left
            origin + widthHalf*left - heightHalf*up + lengthHalf*forward, // back bottom left
            origin - widthHalf*left - heightHalf*up + lengthHalf*forward, // back bottom right
            origin - widthHalf*left + heightHalf*up + lengthHalf*forward, // back top right
            origin + widthHalf*left + heightHalf*up + lengthHalf*forward // back top left
    };
    glm::vec3 normals[6] = {
            glm::normalize(glm::vec3(0) - forward), // backward (~= towards the front)
            glm::normalize(glm::vec3(0) - left), // right
            glm::normalize(forward), // forward (~= towards the back)
            glm::normalize(left), // left
            glm::normalize(up), // up
            glm::normalize(glm::vec3(0) - up) // down
    };
    glm::vec3 tangents[6] = {
            glm::normalize(glm::vec3(0) - left), // backward (~= towards the front)
            glm::normalize(forward), // right
            glm::normalize(left), // forward (~= towards the back)
            glm::normalize(glm::vec3(0) - forward), // left
            glm::normalize(glm::vec3(0) - left), // up
            glm::normalize(left) // down
    };
    std::array<unsigned int, 36> vertexIndices, normalIndices, tangentIndices; // NOLINT (suppressing because we don't want to initialise the yet)
    if(insideOut) { // create faces facing inward
        vertexIndices = {
                0, 2, 1, 0, 3, 2, // front (~= -forward)
                1, 6, 5, 1, 2, 6, // right
                5, 7, 4, 5, 6, 7, // back (~= forward)
                4, 3, 0, 4, 7, 3, // left
                3, 6, 2, 3, 7, 6, // top
                4, 1, 5, 4, 0, 1 // bottom
        };
        normalIndices = {
                2, 2, 2, 2, 2, 2, // back (~= forward)
                3, 3, 3, 3, 3, 3, // left
                0, 0, 0, 0, 0, 0, // front (~= -forward)
                1, 1, 1, 1, 1, 1, // right
                5, 5, 5, 5, 5, 5, // bottom
                4, 4, 4, 4, 4, 4 // top
        };
        tangentIndices = {
                2, 2, 2, 2, 2, 2, // back (~= forward)
                3, 3, 3, 3, 3, 3, // left
                0, 0, 0, 0, 0, 0, // front (~= -forward)
                1, 1, 1, 1, 1, 1, // right
                5, 5, 5, 5, 5, 5, // bottom
                4, 4, 4, 4, 4, 4 // top
        };
    } else { // create faces facing outward
        vertexIndices = {
                0, 1, 2, 0, 2, 3, // front (~= -forward)
                1, 5, 6, 1, 6, 2, // right
                5, 4, 7, 5, 7, 6, // back (~= forward)
                4, 0, 3, 4, 3, 7, // left
                3, 2, 6, 3, 6, 7, // top
                4, 5, 1, 4, 1, 0 // bottom
        };
        normalIndices = {
                0, 0, 0, 0, 0, 0, // front (~= -forward)
                1, 1, 1, 1, 1, 1, // right
                2, 2, 2, 2, 2, 2, // back (~= forward)
                3, 3, 3, 3, 3, 3, // left
                4, 4, 4, 4, 4, 4, // top
                5, 5, 5, 5, 5, 5 // bottom
        };
        tangentIndices = {
                0, 0, 0, 0, 0, 0, // front (~= -forward)
                1, 1, 1, 1, 1, 1, // right
                2, 2, 2, 2, 2, 2, // back (~= forward)
                3, 3, 3, 3, 3, 3, // left
                4, 4, 4, 4, 4, 4, // top
                5, 5, 5, 5, 5, 5 // bottom
        };
    }

    for(auto &index: vertexIndices) {
        tmp = &vertices[index];

        cuboid->vertexData.push_back(tmp->x);
        cuboid->vertexData.push_back(tmp->y);
        cuboid->vertexData.push_back(tmp->z);

        cuboid->colorData.push_back(color.r);
        cuboid->colorData.push_back(color.g);
        cuboid->colorData.push_back(color.b);
        cuboid->colorData.push_back(color.a);
    }
    for(auto &index: normalIndices) {
        tmp = &normals[index];

        cuboid->normalData.push_back(tmp->x);
        cuboid->normalData.push_back(tmp->y);
        cuboid->normalData.push_back(tmp->z);
    }
    for(auto &index: tangentIndices) {
        tmp = &tangents[index];

        cuboid->tangentData.push_back(tmp->x);
        cuboid->tangentData.push_back(tmp->y);
        cuboid->tangentData.push_back(tmp->z);
    }
    for(unsigned int i = 0; i < 36; ++i)
        cuboid->indices.push_back(i); // the vertices are already ordered

    /* fill mesh and add to model */
    cuboid->fillBuffers(false, true, true, true);

    addTo->meshes.push_back(cuboid);
    return addTo;
}

Model *MeshFactory::addCube(Model *addTo,
                            glm::vec3 origin,
                            float size,
                            glm::vec3 forward, glm::vec3 up,
                            glm::vec4 color,
                            bool insideOut) {
    return addCuboid(addTo, origin, size, size, size, forward, up, color, insideOut);
}
