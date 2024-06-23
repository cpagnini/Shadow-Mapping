#include "shader_utils.h"

// Assuming SCR_WIDTH and SCR_HEIGHT are defined in your main file
extern const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

void ConfigureShaderAndMatrices(const Shader& shader) {
    // Configure shader and matrices for both depth and scene shaders
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    shader.use();
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    // Additional configurations for scene shader
    if (&shader == &sceneShader) { // Replace with actual shader comparison method
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
    }
}
