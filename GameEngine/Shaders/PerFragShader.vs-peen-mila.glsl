// Vertex Shader: Executes once for each vertex and 
// is responsible for transforming vertex positions
// and passing along necessary data to the next stages.

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

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 worldPosition;
out vec3 worldNormal;

void main()
{
    worldPosition = modelMatrix * position;
    worldNormal = normalize(mat3(normalModelMatrix) * normal);
    
    // Index into our array using gl_VertexID
    // Calculate the position in clip coordinates
    // MEMORIZE FOR EXAM
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

}