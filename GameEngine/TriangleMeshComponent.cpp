#include "TriangleMeshComponent.h"

TriangleMeshComponent::TriangleMeshComponent(GLuint shaderProgram, int updateOrder)
    : MeshComponent(shaderProgram, updateOrder)
{
};

void TriangleMeshComponent::buildMesh() {
    // create a subMesh struct (declared in MeshComponent)
    // SubMesh objects hold all values involved in rendering of a mesh.
    // Most objects on screen rendered by multiple submeshes.
    // Each MeshComponent can store multiple SubMesh objects in a vector.
    SubMesh subMesh;

	// Generate and bind a vertex array object.
	// Store the identifier for the vertex array object in the subMesh
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	subMesh.primitiveMode = GL_TRIANGLES;
	subMesh.renderMode = ORDERED;
	subMesh.count = 3;
	//subMesh.material.basicColor = BLUE_RGBA;

	// Create a vector to hold vec4 objects for positions
	std::vector<vec4> positions;
	positions.push_back(vec4(-0.25, -0.25, 0.0, 1.0));
	positions.push_back(vec4(0.25, -0.25, 0.0, 1.0));
	positions.push_back(vec4(0.0, 0.25, 0.0, 1.0));

	// Create a buffer and load the vertex positions and colors into it.
	// Store the identifier for the buffer in the subMesh.
	glGenBuffers(1, &subMesh.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, subMesh.vertexBuffer);

	// Copy the data into the buffer object
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), &positions[0], GL_STATIC_DRAW);

	// Specify the location and data format of the vertex positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), 0);
	// Enable the flow of the positions into the vertex shader
	glEnableVertexAttribArray(0);

	// Add to the subMeshes that are part of the mesh component
	subMeshes.push_back(subMesh);

};