#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specualIntensityLocalation, GLuint shinenessLoation);

	~Material();

private:
	GLfloat specularIntensity; //How much reflect the light
	GLfloat shineness; //Size and sharpness of the specular highlights. Higher shininess results smaller
};

