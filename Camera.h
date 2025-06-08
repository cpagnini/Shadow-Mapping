#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

using namespace glm;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    
    Camera(vec3 position = vec3(0.0f, 0.0f, 3.0f),
        vec3 up = vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f);
    
    mat4 GetViewMatrix(); //View matrix
    void ProcessKeyBoard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    

protected:
    //Attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    //Angles
    float Yaw, Pitch;

    //Parameters
    float MovementSpeed, MouseSensivity, Zoom;

private:
    void updateCamerVectors();
};

