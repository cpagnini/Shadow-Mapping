#version 330 core

layout (location = 0) in vec3 aPos;      // vertex position
layout (location = 1) in vec3 aNormal;   // vertex normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;       // world position
out vec3 Normal;        // world normal

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));       // transforming into world coords
    Normal = mat3(transpose(inverse(model))) * aNormal; // even normals

    gl_Position = projection * view * vec4(FragPos, 1.0); // clip space
}
