#version 330 core

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec2 TexCoords;

// Scene Uniforms
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightDir;

// Material/Object Uniforms
uniform vec3 floorColor;
uniform vec3 sphereColor;
uniform bool isFloor;

// Textures
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

out vec4 FragColor;

//Step 0. The Shadow:A point P is in shadow if there is another point Q such that
//Q is closer to the light than P along the same direction.
// Helper function to calculate shadow factor based on a given bias
float CalculateShadow(vec3 projCoords, float bias)
{
    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // Check whether current frag pos is in shadow
    return (currentDepth - bias) > closestDepth ? 1.0 : 0.0;
}

void main()
{
    // 1. Setup Base Color and Vectors
    vec3 color = isFloor ? floorColor : sphereColor;
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir); // Direction FROM fragment TO light
    vec3 V = normalize(viewPos - FragPos);
    
    // 2. Shadow Mapping Coordinates (Perspective Divide + Transform to [0,1])
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // 3. Lighting components (Phong/Blinn-Phong)
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    float diff = max(dot(N, L), 0.0); //Lambert law
    vec3 diffuse = diff * lightColor;

    // Specular (Disabled for the floor to simulate different materials)
    vec3 specular = vec3(0.0);
    if (!isFloor) {
        vec3 halfwayDir = normalize(L + V);
        float spec = pow(max(dot(N, halfwayDir), 0.0), 720.0);
        specular = spec * lightColor;
    }

    float segment = 1980.0 / 3.0;
    float currentX = gl_FragCoord.x;

    // ---  FILTERS --

    if (currentX < segment) {
        // Visione as X Ray / Depth Map
        float depthValue = texture(shadowMap, projCoords.xy).r;
        FragColor = vec4(vec3(depthValue), 1.0);
    } 
    else if (currentX < segment * 2.0) {
        // No  Bias (Acne visibile)
        float bias = 0.0;
        float shadow = CalculateShadow(projCoords, bias);
        
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * lightColor;
        
        vec3 result = (ambient + (1.0 - shadow) * diffuse) * color;
        FragColor = vec4(result, 1.0);
    } 
    else {
        // (Bias + PCF)
        float bias = max(0.01 * (1.0 - dot(N, L)), 0.005);
        float shadow = 0.0;
        float currentDepth = projCoords.z;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x) {
            for(int y = -1; y <= 1; ++y) {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;
        //float shadow = CalculateShadow(projCoords, bias); 
        
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;
        float diff = max(dot(N, L), 0.0);
        vec3 diffuse = diff * lightColor;
        
        if(projCoords.z > 1.0)
            shadow = 0.0;

        vec3 result = (ambient + (1.0 - shadow) * diffuse) * color;
        FragColor = vec4(result, 1.0);
    }
}