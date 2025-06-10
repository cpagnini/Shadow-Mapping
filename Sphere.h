#pragma once
#include "Mesh.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

//Since we cannot draw all the points on a sphere, 
// we only sample a limited amount of points by dividing the sphere by sectors (longitude) and stacks (latitude)
class Sphere:public Mesh
{
public:
	//Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, bool smooth = true, int up = 3);
	Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
	~Sphere() {}
	void draw(Shader& shader);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
};

