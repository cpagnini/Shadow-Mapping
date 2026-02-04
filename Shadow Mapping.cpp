#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Plane.h"
#include "ShadowMap.h"
#include "Skybox.h"
#include "Texture.h"

using namespace glm;

const int WIDTH = 1980;
const int HEIGHT = 1080;
float fov = 45.0f;
float deltaTime, lastFrame;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera) {


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = WIDTH / 2.0f;
    static float lastY = HEIGHT / 2.0f;

    if (firstMouse) //Prevent huge calculation of the mouse pos
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; //Y position are growing going down

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

int main()
{
    glfwInit();
    //GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    //Creating the view
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Shadow Mapping", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    

    void framebuffer_size_callback(GLFWwindow * window, int width, int height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //Texture
    Texture text;
    int mapId = text.loadCubeMap();
    //Skybox
    Skybox skybox{};
    skybox.setTexture(mapId);
    
    //Plane
    Plane floor(20.0f, 20.0f);
    //Spheres
    Sphere sphere1(1.0f, 36, 18); // Big
    Sphere sphere2(0.6f, 36, 18); // Medium
    Sphere sphere3(0.3f, 36, 18); // Small

    //Shaders
    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    Shader shadowShader("shaders/shadow.vert", "shaders/shadow.frag");


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Nasconde il mouse

 
    ShadowMap map;
    map.init(WIDTH, HEIGHT);


    //Main loop

    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera);

        float r1 = sphere1.getRadius();
        float r2 = sphere2.getRadius();
        float r3 = sphere3.getRadius();

        std::vector<glm::vec3> spherePositions = {
            glm::vec3(-2.0f, r1 - 1.0f, 0.0f), // left
            glm::vec3(0.0f, r2 - 1.0f, 0.0f),  // middle
            glm::vec3(2.0f, r3 - 1.0f, 0.0f)   // right
        };

        // ---------- 1. SHADOW PASS ----------
        glCullFace(GL_FRONT);
        glViewport(0, 0, map.getShadowWidth(), map.getShadowHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, map.depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowShader.use();
        shadowShader.setMat4("lightSpaceMatrix", map.getLightSpaceMatrix());

        glm::mat4 modelSphere = glm::translate(glm::mat4(1.0f), spherePositions[0]);
        shadowShader.setMat4("model", modelSphere);
        sphere1.draw(shadowShader, spherePositions[0]);

        float time = (float)glfwGetTime();

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 modelS2 = glm::mat4(1.0f);
        modelS2 = glm::rotate(modelS2, time, glm::vec3(0.0f, 1.0f, 0.0f));
        modelS2 = glm::translate(modelS2, spherePositions[1]);
        shadowShader.setMat4("model", modelS2);
        sphere2.drawR(shadowShader, modelS2);


        modelSphere = glm::translate(glm::mat4(1.0f), spherePositions[2]);
        shadowShader.setMat4("model", modelSphere);
        sphere3.draw(shadowShader, spherePositions[2]);

        glm::mat4 modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shadowShader.setMat4("model", modelFloor);
        floor.Draw(shadowShader);

        glm::mat4 modelSkybox = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shadowShader.setMat4("model", modelSkybox);
        skybox.Draw(modelSkybox);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        // ---------- 2. MAIN RENDER PASS ----------
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("lightDir", map.getLightDir());
        //shader.setVec3("lightPos", glm::vec3(-2.0f, 4.0f, -1.0f));
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        //shader.setVec3("objectColor", glm::vec3(0.8f, 0.2f, 0.3f));
        shader.setVec3("floorColor", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("sphereColor", glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("lightSpaceMatrix", map.getLightSpaceMatrix());

        float aspectRatio = static_cast<float>(WIDTH) / HEIGHT;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("lightSpaceMatrix", map.getLightSpaceMatrix());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, map.depthMap);
        shader.setInt("shadowMap", 1);

        

        shader.setBool("isFloor", false);
        model = glm::translate(glm::mat4(1.0f), spherePositions[0]);
        shader.setMat4("model", model);
        sphere1.draw(shader, spherePositions[0]);

        glm::mat4 modelS2_main = glm::mat4(1.0f);
        modelS2_main = glm::rotate(modelS2_main, time, glm::vec3(0.0f, 1.0f, 0.0f));
        modelS2_main = glm::translate(modelS2_main, spherePositions[1]);
        shader.setMat4("model", modelS2_main);
        sphere2.drawR(shader, modelS2_main);

        model = glm::translate(glm::mat4(1.0f), spherePositions[2]);
        shader.setMat4("model", model);
        sphere3.draw(shader, spherePositions[2]);

        shader.setBool("isFloor", true);
        modelFloor = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.setMat4("model", modelFloor);
        floor.Draw(shader);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwTerminate();

    return 0;
}

