#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Mesh() {};
    /// <summary>
    /// 
    /// </summary>
    /// <param name="vertices"></param>
    /// <param name="indices"></param>
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="shader"></param>
    void Draw(Shader& shader);

protected:
    unsigned int VAO,VBO, EBO;
    void setupMesh(); 
};
