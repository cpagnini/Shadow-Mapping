#include "glm/glm.hpp"

#pragma once
class ShadowMap
{
public:
	unsigned int depthMapFBO;
	unsigned int depthMap;
	


	void init(unsigned int width, unsigned int height);

	void ConfigureShaderAndMatrices();

	glm::mat4 getLightSpaceMatrix() const;
	glm::vec3 getLightDir() const;
	int getShadowWidth() const;
	int getShadowHeight() const;

protected:
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightDir;
	unsigned int shadowWidth;
	unsigned int shadowHeight;

};

