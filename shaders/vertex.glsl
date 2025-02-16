#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
flat out uint Chunk;
// out mat3 TBN;  

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

// uniform vec3 tangents[12];
// uniform vec3 bitangents[12];

void main()
{

    FragPos = aPos;
    Normal = aNormal;
    TexCoords = aTexCoords; 

    // int i = gl_VertexID / 3;
    // vec3 tangent = tangents[i];
    // vec3 bitangent = bitangents[i];

    // vec3 T = normalize(vec3(model * vec4(tangent,0.0)));
    // vec3 B = normalize(vec3(model * vec4(bitangent,0.0)));
    // vec3 N = normalize(vec3(model * vec4(aNormal,0.0)));
    // TBN = mat3(T, B, N);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);

}