// #version 330 core
// out vec4 FragColor;



// uniform vec3 lightPos; 
// uniform vec3 viewPos; 
// uniform vec3 lightColor;
// uniform vec3 objectColor;

// void main()
// {

//     // obtain normal from normal map in range [0,1]
//     vec3 norm = texture(normalMap, TexCoords).rgb;
//     // transform normal vector to range [-1,1]
//     norm = normalize(norm * 2.0 - 1.0);   
//     float ambientStrength = 0.1;
//     vec3 ambient = ambientStrength * lightColor;
  	
//     // diffuse 
//     vec3 lightDir = normalize(lightPos - FragPos);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;
    
//     // specular
//     float specularStrength = 0.5;
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);  
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
//     vec3 specular = specularStrength * spec * lightColor;  
        
//     vec3 result = (ambient + diffuse + specular);
//     FragColor = texture(image, TexCoords) * vec4(result, 1.0);
// }

#version 330 core
out vec4 FragColor;

uniform sampler2D image;
uniform sampler2D normalMap;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
in mat3 TBN;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{

    // vec3 normal = texture(normalMap, TexCoords).rgb;
    // normal = normal * 2.0 - 1.0;
    // // transform normal vector to range [-1,1]
    // normal = normalize(TBN * normal);  
    vec3 normal = normalize(Normal);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normal;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) ;
    FragColor = texture(image,TexCoords) * vec4(result, 1.0);
} 