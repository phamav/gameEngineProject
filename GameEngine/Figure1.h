#pragma once
#include "MeshComponent.h"

class Figure1 :
    public MeshComponent
{
public:
    Figure1(GLuint shaderProgram, int updateOrder = 100);
    virtual void buildMesh() override;
};

