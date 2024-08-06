#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    /// <summary>
    /// 
    /// </summary>
    /// <param name="red"></param>
    /// <param name="green"></param>
    /// <param name="blue"></param>
   /// <param name="aIntensity">Ambient light intensity: the overall brightness of the light; indirect light in the scene.</param>
    /// <param name="dIntensity">Diffuse light intensity: the strength of light that affects surfaces based on their orientation</param>
    /// <param name="xDir">The x-component of the light's direction vector.</param>
    /// <param name="yDir">The y-component of the light's direction vector.</param>
    /// <param name="zDir">The z-component of the light's direction vector.</param>
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,  GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);
    
    /// <summary>
    /// Sets the light properties in the shader program.
    /// Uploads the light's color, intensities, and direction to the shader using uniform variables.
    /// </summary>
    /// <param name="ambientIntensityLocation">Location represents the intensity of the ambient light affecting the scene.</param>
    /// <param name="ambientColourLocation">Location represents the color of the ambient light.</param>
    /// <param name="diffuseIntensityLocation">Location represents the strength of the light affecting surfaces based on their orientation.</param>
    /// <param name="directionLocation">Location  represents the direction from which the light is coming, important for diffuse lighting calculations.</param>
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);


    ~DirectionalLight();

private:
    glm::vec3 direction;
};

