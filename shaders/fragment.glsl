#version 330 core
out vec4 FragColor;

uniform sampler2D textureAtlas;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
// in mat3 TBN;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{

    // vec3 normal = texture(normalMap, TexCoords).rgb;
    // normal = normal * 2.0 - 1.0;

    // normal = normalize(TBN * normal);  
    float gamma = 2.2;
    vec3 normal = normalize(Normal);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - FragPos);
  	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse 

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    // float max_distance = 1.5;
    // float distance = length(lightPos - FragPos);
    // float attenuation = 1.0 / distance;

    // diffuse *= attenuation;
    // specular *= attenuation;

    vec3 result = (ambient + diffuse + specular); 
    vec3 color = texture(textureAtlas,TexCoords).rgb;
    color = color * result;
    color = pow(color, vec3(1.0/gamma));
    FragColor = vec4(color, 1.0);
} 