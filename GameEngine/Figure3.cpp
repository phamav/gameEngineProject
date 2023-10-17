#include "Figure3.h"

Figure3::Figure3(GLuint shaderProgram, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder)
{
};

void Figure3::buildMesh() {
	// Create a vector to hold vec4 objects for positions
	std::vector<pntVertexData> vertexData;
	vertexData.push_back(pntVertexData(vec4(0.0, 0.75, 0.0, 1.0))); // 0
	vertexData.push_back(pntVertexData(vec4(-0.25, 0.25, 0.0, 1.0))); // 1
	vertexData.push_back(pntVertexData(vec4(-0.75, 0.0, 0.0, 1.0))); // 2
	vertexData.push_back(pntVertexData(vec4(-0.25, -0.25, 0.0, 1.0))); // 3
	vertexData.push_back(pntVertexData(vec4(0.0, -0.75, 0.0, 1.0))); // 4
	vertexData.push_back(pntVertexData(vec4(0.25, -0.25, 0.0, 1.0))); // 5
	vertexData.push_back(pntVertexData(vec4(0.75, 0.0, 0.0, 1.0))); // 6
	vertexData.push_back(pntVertexData(vec4(0.25, 0.25, 0.0, 1.0))); // 7

	std::vector<GLuint> indices;
	// top
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(7);

	// left
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// bottom
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(5);

	// right
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	// inner
	indices.push_back(7);
	indices.push_back(1);
	indices.push_back(5);

	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(5);
	
	// create a subMesh struct (declared in MeshComponent)
	// SubMesh objects hold all values involved in rendering of a mesh.
	// Most objects on screen rendered by multiple submeshes.
	// Each MeshComponent can store multiple SubMesh objects in a vector.
	SubMesh subMesh = buildSubMesh(vertexData, indices);

	subMesh.primitiveMode = GL_TRIANGLES;
	//subMesh.material.basicColor = BLACK_RGBA;

	// Add to the subMeshes that are part of the mesh component
	subMeshes.push_back(subMesh);
};