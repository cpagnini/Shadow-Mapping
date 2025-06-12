
#include "Sphere.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.hpp>

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    this->radius = radius;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float pi = glm::pi<float>();
    float sectorStep = 2 * pi / sectorCount;
    float stackStep = pi / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = pi / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            Vertex vertex;
            vertex.Position = glm::vec3(x, y, z);
            vertex.Normal = glm::vec3(nx, ny, nz);
            vertex.TexCoords = glm::vec2(s, t);
            vertices.push_back(vertex);
        }
    }

    for (unsigned int i = 0; i < stackCount; ++i) {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    // Ora la Mesh viene costruita correttamente e chiama setupMesh()
    mesh = Mesh(vertices, indices);
}

void Sphere::draw(Shader& shader, glm::vec3 position) {
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    shader.setMat4("model", model);
    mesh.Draw(shader);
}

float Sphere::getRadius() const
{
    return this->radius;
}

