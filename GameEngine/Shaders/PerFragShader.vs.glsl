// Targeting version 4.5 of GLSL. 
#version 460 core

// Uniform block
layout(shared) uniform transformBlock
{
    mat4 modelMatrix;
    mat4 normalModelMatrix;
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

// world block
layout(shared) uniform worldEyeBlock
{
    vec3 worldEyePosition;
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 worldPos;
out vec3 worldNorm;

void main()
{
     // Index into our array using gl_VertexID
     // Calculate the position in clip coordinates
     // MEMORIZE FOR EXAM
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

     // Transform the position of the vertex to world 
    // coords for lighting
    worldPos = (modelMatrix * position);

    // Transform the normal to world coords for lighting
    worldNorm = normalize(mat3(normalModelMatrix) * normal); 

}