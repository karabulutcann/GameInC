#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;
    float Linear;
    float Quadratic;
    int type;
};

const int NR_LIGHTS = 1;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.7; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {

        // diffuse
        vec3 lightDir = vec3(0.0, 0.0, 0.0);
        if(lights[i].type == 1)
        {
            lightDir = normalize(-lights[i].Position);
        }
        else
        {
            lightDir = normalize(lights[i].Position - FragPos);
        }
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 512.0);
        vec3 specular = lights[i].Color * spec * Specular;

        // attenuation
        if(lights[i].type != 1){    
            float distance = length(lights[i].Position - FragPos);
            float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
        }

        lighting += diffuse + specular;        
    }
    FragColor = vec4(lighting, 1.0);
}