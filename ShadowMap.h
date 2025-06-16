#pragma once
class ShadowMap
{
public:
	unsigned int depthMapFBO;
	unsigned int depthMap;

	void init(unsigned int width, unsigned int height);

};

