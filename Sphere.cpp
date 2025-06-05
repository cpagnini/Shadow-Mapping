
#include "Sphere.h"
#include "Mesh.h"
#include <cmath>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount)
    : Mesh({},{}) // Mesh initialized with empty vectors
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float x, y, z, xy;                              // coords
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normals
    float s, t;                                      // texture

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);              // r * cos(u)
        z = radius * sinf(stackAngle);               // r * sin(u)

        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

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

    // Indexes
    for (unsigned int i = 0; i < stackCount; ++i)
    {
        unsigned int k1 = i * (sectorCount + 1);     // stack
        unsigned int k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
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

    // Copying inside the mesh
    this->vertices = vertices;
    this->indices = indices;
    this->setupMesh();
}
