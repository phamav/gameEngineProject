#pragma once
#include "MeshComponent.h"

class TriangleMeshComponent :
    public MeshComponent
{
public:
    TriangleMeshComponent(GLuint shaderProgram, int updateOrder = 100);
    virtual void buildMesh() override;
};

