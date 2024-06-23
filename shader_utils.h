#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"  // Include your Shader class header

extern const unsigned int SCR_WIDTH, SCR_HEIGHT;

void ConfigureShaderAndMatrices(const Shader& shader);

#endif // SHADER_UTILS_H
