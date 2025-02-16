#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

in uint chunkData[];
// out vec3 fragColor; // Pass color to fragment shader

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
    vec4 pos = gl_in[0].gl_Position; // Input point position
    float size = 0.2; // Cube half-size
    
    // Define cube vertices relative to point position
    vec4 vertices[8] = vec4[](
        pos + vec4(-size, -size, -size, 0.0), 

        pos + vec4( size, -size, -size, 0.0),

        pos + vec4( size,  size, -size, 0.0),

        pos + vec4(-size,  size, -size, 0.0),
        pos + vec4(-size, -size,  size, 0.0),
        pos + vec4( size, -size,  size, 0.0),
        pos + vec4( size,  size,  size, 0.0),
        pos + vec4(-size,  size,  size, 0.0)
    );

    vec2 TexCoordsArr[4] = vec2[](
        vec2(1.0, 0.0),
        vec2(1.0, 1.0),
        vec2(0.0, 1.0),
        vec2(0.0, 0.0)
    );

    vec3 normalArr[6] = vec3[](
        vec3(0.0f,  0.0f, -1.0f),
        vec3(0.0f,  0.0f, 1.0f),
        vec3(-1.0f,  0.0f, 0.0f),
        vec3(1.0f,  0.0f, 0.0f),
        vec3(0.0f,  -1.0f, 0.0f),
        vec3(0.0f,  1.0f, 0.0f)
    );

    // Cube faces (two triangles per face)

    int indices[36] = int[](
    0, 1, 2,  2, 3, 0,
    4, 5, 6,  6, 7, 4,
    7, 3, 0,  0, 4, 7,
    6, 2, 1,  1, 5, 6,
    0, 1, 5,  5, 4, 0,
    3, 2, 6,  6, 7, 3
    );

    int textCordIndices[36] = int[](
        3, 0, 1,  1, 2, 3, 
        3, 0, 1,  1, 2, 3,
         0, 1, 2,  2, 3, 0,
          0, 1, 2, 2, 3, 0,
           2, 1, 0,  0, 3, 2, 
           2, 1, 0,  0, 3, 2
    );

    mat3 TBNs[6];

for (int i = 0; i < 6; i++) {
     vec3 edge1 = vertices[indices[i * 6 +1]].xyz - vertices[indices[i * 6]].xyz;
    vec3 edge2 = vertices[indices[i * 6 +2]].xyz - vertices[indices[i * 6]].xyz;
    vec2 deltaUV1 = TexCoordsArr[textCordIndices[i * 6 +1]] - TexCoordsArr[textCordIndices[i * 6]];
    vec2 deltaUV2 = TexCoordsArr[textCordIndices[i * 6 +2]] - TexCoordsArr[textCordIndices[i * 6]];

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    vec3 tangent = vec3(f * (deltaUV2.y * edge1 -deltaUV1.y *edge2));
    vec3 bitangent = vec3(f * (-deltaUV2.x * edge1 + deltaUV1.x *edge2));

    vec3 T = normalize(vec3(model * vec4(tangent,0.0f)));
    vec3 B = normalize(vec3(model * vec4(bitangent,0.0f)));
    vec3 N = normalize(vec3(model * vec4(cross(edge1,edge2),0.0f)));

    TBNs[i] = mat3(T,B,N);
}
   

    // Emit cube vertices
    for (int i = 0; i < 36; i++) {
        int normaldd = 0;
        if(i>5 && i<12){
            normaldd = 1;
        }else if(i>11 && i<18){
            normaldd = 2;
        }else if(i>17 && i<24){
            normaldd = 3;
        }else if(i>23 && i<30){
            normaldd = 4;
        }else if(i>29 && i<36){
            normaldd = 5;
        }
        // model matrixi ile carpmiyorsun duzelt
        FragPos = vec3(vertices[indices[i]]);
        Normal = mat3(transpose(inverse(model))) * normalArr[normaldd]; 
        TBN = TBNs[normaldd];
        gl_Position = projection * view * vertices[indices[i]];
        TexCoords = TexCoordsArr[textCordIndices[i]];
        // fragColor = vec3(1.0, 0.5, 0.2); // Set color
        if(chunkData[0] == 0u){
            EmitVertex();
        }

        if ((i + 1) % 3 == 0) EndPrimitive(); // End each triangle
    }
}
