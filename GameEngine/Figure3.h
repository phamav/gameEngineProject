#pragma once
#include "MeshComponent.h"

class Figure3 :
    public MeshComponent
{
public:
    Figure3(GLuint shaderProgram, int updateOrder = 100);
    virtual void buildMesh() override;
};
