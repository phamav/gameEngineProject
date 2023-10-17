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

// setting material properties via a uniform block
layout(shared) uniform materialBlock
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

//out vec4 worldPos;
//out vec3 worldNorm;

out vec4 vsColor;

void main()
{
    // Index into our array using gl_VertexID
    // Calculate the position in clip coordinates
    // MEMORIZE FOR EXAM
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

     // Transform the position of the vertex to world 
    // coords for lighting
    vec4 worldPos = (modelMatrix * position);

    // Transform the normal to world coords for lighting
    vec3 worldNorm = normalize(mat3(normalModelMatrix) * normal); 

    // set light position
	vec4 posLight = vec4(0.5f, 10.0f, 10.0f, 1.0f);
	vec4 dirLight = normalize(vec4(10.0f, 10.0f, 10.0f, 1.0f));

	// hardcoded material properties
	// TODO: add into a uniform block material
//	vec4 ambientMat = vec4(0.1f, 0.1f, 0.1f, 1.0f);
//	vec4 diffuseMat = vec4(0.0f, 0.0f, 1.0f, 1.0f); // blue
//	vec4 specularMat = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//	vec4 emmissiveMat = vec4(0.0f, 0.0f, 0.1f, 1.0f); 
//	float specularExp = 32.0f;

	// Hardcoded light properties
	vec4 ambientLight = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 diffuseLight = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specularLight = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Normalize normals
	vec3 norm = normalize(worldNorm);
	vec3 viewVector = normalize(worldEyePosition - worldPos.xyz);

	// Positional light calculations
	vec3 lightVector = normalize(posLight.xyz - worldPos.xyz);
	vec3 reflectionVec = normalize(reflect(-lightVector, norm));
	vec4 ambient = ambientMat * ambientLight;
	vec4 diffuse = max(dot(lightVector, norm), 0.0f) * diffuseLight * diffuseMat;
	vec4 specular = pow(max(dot(viewVector, reflectionVec), 0.0f), specularExp) * specularLight * specularMat;
	vec4 illumColor = ambient + diffuse + specular + emmissiveMat;

	// Directional light calculations
	vec3 dirLightVector =  dirLight.xyz;
	vec3 dirReflectVec = normalize(reflect(-dirLightVector, norm));
	vec4 diffuseDir = max(dot(dirLightVector, norm), 0.0f) * diffuseLight * diffuseMat;
	vec4 specularDir = pow(max(dot(viewVector, dirReflectVec), 0.0f), specularExp) * specularLight * specularMat;
	vec4 dirLightColor = ambient + + diffuseDir + specularDir + emmissiveMat;
	
	// get the final color
	vsColor = illumColor + dirLightColor;
}