#include "Figure2.h"

Figure2::Figure2(GLuint shaderProgram, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder)
{
};

void Figure2::buildMesh() {
	//  Create a vector of pntVertexData for vertex positions
	std::vector<pntVertexData> vertexData;

	// Add vertex positions for the triangle strip
	vertexData.push_back(pntVertexData(vec4(-0.75, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.75, -0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.5, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(-0.25, -0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.0, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.25, -0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.5, 0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.75, -0.5, 0.0, 1.0)));
	vertexData.push_back(pntVertexData(vec4(0.75, 0.5, 0.0, 1.0)));

	// Use the buildSubMesh function
	SubMesh subMesh = buildSubMesh(vertexData);
	//subMesh.material.basicColor = YELLOW_RGBA;
	subMesh.primitiveMode = GL_TRIANGLE_STRIP;

	// add to the subMeshes that are part of the mesh component
	subMeshes.push_back(subMesh);

};