#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec2 TexCoords;

uniform vec3 lightPos;   // posizione luce
uniform vec3 viewPos;    // posizione camera
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

out vec4 FragColor;


float ShadowCalculation(vec4 fragPosLightSpace,vec3 lightDir,vec3 norm)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005); 
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 lightColor = vec3(1.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // colore finale
    float shadow = ShadowCalculation(FragPosLightSpace,lightDir, norm);       
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 
    //vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}