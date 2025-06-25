#include "ShadowMap.h"
#include <glad/glad.h>
#include <cstddef>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void ShadowMap::init(unsigned int width, unsigned int height)
{
	shadowWidth = SHADOW_WIDTH;
	shadowHeight = SHADOW_HEIGHT;
	//Frame buffer
	glGenFramebuffers(1, &depthMapFBO);

	//Texture depth
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//Here we need to tell explicity to OpenGL that we're not going to render any color data
	glDrawBuffer(GL_NONE); //No color data
	glReadBuffer(GL_NONE); //No color data
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Step 1: Generate the depth map 
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	ConfigureShaderAndMatrices();
	//RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Step 2: Render the scene height depth map
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ConfigureShaderAndMatrices();
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//RenderScene();
	
}

//We are modelling a directional light source and all its light rays are parallel. That's why we have to use orthographics projection matrix for the light source
void ShadowMap::ConfigureShaderAndMatrices() {
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//Since the projection matrix indirectly determines the range of what is visibile, you have to make sure that the size of the projection frustum correctly contains the objects that has to be inside the depth map.
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	lightSpaceMatrix = lightProjection * lightView;
}

glm::mat4 ShadowMap::getLightSpaceMatrix()  const{
	return lightSpaceMatrix;
}

int ShadowMap::getShadowWidth() const
{
	return shadowWidth;
}

int ShadowMap::getShadowHeight() const
{
	return shadowHeight;
}
