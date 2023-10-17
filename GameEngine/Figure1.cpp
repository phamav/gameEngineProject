#include "Figure1.h"

Figure1::Figure1(GLuint shaderProgram, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder)
{
};

void Figure1::buildMesh() {
	// Create a vector of pntVertexData for vertex positions
	std::vector<pntVertexData> vertexData;

	// T1
	vertexData.push_back(pntVertexData(vec4(-0.5, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.7, 0.0, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.3, 0.0, 0.0, 1.0)));

	// T2
	vertexData.push_back(pntVertexData(vec4(0.0, -0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.2, 0.0, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.2, 0.0, 0.0, 1.0)));

	// T3
	vertexData.push_back(pntVertexData(vec4(0.5, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.3, 0.0, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.7, 0.0, 0.0, 1.0)));

	// create a subMesh struct (declared in MeshComponent)
	// SubMesh objects hold all values involved in rendering of a mesh.
	// Most objects on screen rendered by multiple submeshes.
	// Each MeshComponent can store multiple SubMesh objects in a vector.
	
	// Use the buildSubMesh function in MeshComponent
	// to create a subMesh with the vertex data
	SubMesh subMesh = buildSubMesh(vertexData);

	subMesh.primitiveMode = GL_TRIANGLES;
	//subMesh.material.basicColor = GREEN_RGBA;

	// Add to the subMeshes that are part of the mesh component
	subMeshes.push_back(subMesh);
};