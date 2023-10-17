#pragma once

#include "MathLibsConstsFuncs.h"

using namespace constants_and_types;

// Everything in struct has public access.
// Everything in Material struct can be accessed and modified from
// outside of the struct.
struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
};
