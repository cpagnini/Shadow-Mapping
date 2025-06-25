#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform vec3 lightPos;   // posizione luce
uniform vec3 viewPos;    // posizione camera
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D shadowMap;


out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // Normalizza coordinate in [0,1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Prendi la profondità più vicina dalla shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // Profondità corrente
    float currentDepth = projCoords.z;

    // Se siamo più lontani della profondità nella shadow map, siamo in ombra
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    // luce ambientale
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // luce diffusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    float shadow = ShadowCalculation(FragPosLightSpace);


    vec3 diffuse = diff * lightColor;

    // colore finale
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
