#include "Plane.h"
#include <glm/glm.hpp>

Plane::Plane(float width, float depth) {
    std::vector<Vertex> vertices(4);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;

    // Posizioni
    vertices[0].Position = glm::vec3(-halfWidth, 0.0f, -halfDepth);
    vertices[1].Position = glm::vec3(halfWidth, 0.0f, -halfDepth);
    vertices[2].Position = glm::vec3(halfWidth, 0.0f, halfDepth);
    vertices[3].Position = glm::vec3(-halfWidth, 0.0f, halfDepth);

    // Normali (tutte verso l'alto)
    for (int i = 0; i < 4; ++i) {
        vertices[i].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // TexCoords (per texture mapping se serve)
    vertices[0].TexCoords = glm::vec2(0.0f, 0.0f);
    vertices[1].TexCoords = glm::vec2(1.0f, 0.0f);
    vertices[2].TexCoords = glm::vec2(1.0f, 1.0f);
    vertices[3].TexCoords = glm::vec2(0.0f, 1.0f);

    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
}
