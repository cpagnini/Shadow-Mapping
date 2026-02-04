#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

class Skybox {
private:
    unsigned int VAO, VBO;
    unsigned int cubemapID;
    
    void setupSkybox();

public:
    Skybox(); 
    std::vector<float> getSkyboxVertices();
    void Draw(Shader& shader, glm::mat4 view, glm::mat4 projection);
    void setTexture(unsigned int id) { cubemapID = id; }
};
