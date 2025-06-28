#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;   // posizione luce
uniform vec3 viewPos;    // posizione camera
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    // luce ambientale
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // luce diffusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // colore finale
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}