#version 330 core

layout (location = 0) in vec3 aPos;      // vertex position
layout (location = 1) in vec3 aNormal;   // vertex normal
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;       // world position
out vec3 Normal;        // world normal
out vec4 FragPosLightSpace;
out vec2 TexCoords;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));       // transforming into world coords
    Normal = mat3(transpose(inverse(model))) * aNormal; // even normals
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0); //transform vertices to light space in the depth map stage
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0); // clip space
}
