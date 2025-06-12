#pragma once

#include "Mesh.h"
#include "Shader.h"

class Sphere {
public:
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
    void draw(Shader& shader, glm::vec3 position);
    float getRadius() const;

    
private:
    Mesh mesh;
    float radius;
};
