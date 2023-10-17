// Fragment (pixel) shader: Executes for each pixel
// that's going to be drawn on the screen.
// Determines the final color of the pixel.


// Targeting version 4.5 of GLSL. 
#version 460 core

// World coordinate fragment position and normal
in vec4 worldPosition;
in vec3 worldNormal;

layout(location = 500) uniform vec4 vsColor;

out vec4 fragmentColor;

layout(shared) uniform MaterialBlock
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
};

// worldEyePosition
layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

// Light sources properties
vec4 ambientColor;
vec4 diffuseColor;
vec4 specularColor;

vec4 positionalLight = vec4(0.0f, 10.0, 10.0f, 1.0f);
vec4 directionalLight = vec4(10.0f, 10.0, 10.0f, 1.0f);


void main()
{
	// set light properties
	ambientColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	diffuseColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

//	// set object properties
//	ambientMat = vec4(0.0f, 0.15f, 0.0f, 1.0f);
//	diffuseMat = vec4(0.0f, 1.0f, 0.0f, 1.0f);
//	specularMat = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//	emmissiveMat = vec4(0.0f, 1.0f, 0.0f, 1.0f);
//
//	specularExp = 256;
//
//	vec4 illumColor = emmissiveMat;
//	illumColor += ambientColor * ambientMat;
//
//	lightVector = normalize(lightPosition.xyz - worldPosition.xyz);
//	vec3 viewVector = normalize(worldEyePosition - worldPosition.xyz);
//
//	illumColor += max(dot(lightVector, worldNormal), 0.0f) * diffuseColor * diffuseMat;
//
//	vec3 reflectionVec = reflect(-lightVector, worldNormal); 
//	fragmentColor = illumColor
// Normalize worldNormal
	vec3 norm = normalize(worldNormal);

	// Light calculation for positional light
	vec3 viewVector = normalize(worldEyePosition - worldPosition.xyz);
	vec3 lightVector = normalize(positionalLight.xyz - worldPosition.xyz);
	vec3 reflectionVec = normalize(reflect(-lightVector, norm));

	vec4 illumColor = emmissiveMat;
	illumColor += ambientColor * ambientMat;
	illumColor += max(dot(lightVector, norm), 0.0f) * diffuseColor * diffuseMat;
	illumColor += pow(max(dot(viewVector, reflectionVec), 0.0f), specularExp) * specularColor * specularMat;

	// Light calculation for directional light
	vec3 dirLightVec = normalize(vec3(3.5f, 5.0f, 2.5f));
	vec3 dirReflectVec = normalize(reflect(-dirLightVec, norm));

	vec4 dirLightColor = emmissiveMat;
	dirLightColor += ambientColor * ambientMat;
	dirLightColor += max(dot(dirLightVec, norm), 0.0f) * diffuseColor * diffuseMat;
	dirLightColor += pow(max(dot(viewVector, dirReflectVec), 0.0f), specularExp) * specularColor * specularMat;

	fragmentColor = illumColor + dirLightColor;


}