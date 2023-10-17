#pragma once

#define materialBlockBindingPoint 20

#include "SharedUniformBlock.h"
#include "MathLibsConstsFuncs.h"
#include "Material.h"

using namespace constants_and_types;

class SharedMaterials
{
public:

    static void setUniformBlockForMaterial(GLuint shaderProgram);
    static void setShaderMaterialProperties(const Material& material);

protected:

    static GLuint ambientLocation;
    static GLuint diffuseLocation;
    static GLuint specularLocation;
    static GLuint emmissiveLocation;
    static GLuint specularExpLocation;

    // current data held in buffer
    static vec4 ambientMat;
    static vec4 diffuseMat;
    static vec4 specularMat;
    static vec4 emmissiveMat;
    static float specularExp;

    static SharedUniformBlock materialBlock;
    static const std::string materialBlockName;
};
