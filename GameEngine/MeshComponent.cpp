#include "MeshComponent.h"
#include "SharedMaterials.h"

#define VERBOSE false

/** @brief	Definition of a static data member; meshComps */
// std::vector<class MeshComponent*> MeshComponent::meshComps;
std::vector<std::shared_ptr<class MeshComponent>> MeshComponent::meshComps;

MeshComponent::~MeshComponent()
{
	if (VERBOSE) cout << "MeshComponent destructor called " << endl;

	for (auto& subMesh : subMeshes) {

		// Delete the VAO
		if (subMesh.vao)
		{
			glDeleteVertexArrays(1, &subMesh.vao);
			subMesh.vao = 0;
		}

		// Delete the vertex buffer
		if (subMesh.vertexBuffer) {
			glDeleteBuffers(1, &subMesh.vertexBuffer);
			subMesh.vertexBuffer = 0;
		}

		// Delete the index buffer
		if (subMesh.renderMode == INDEXED && subMesh.indexBuffer) {
			glDeleteBuffers(1, &subMesh.indexBuffer);
			subMesh.indexBuffer = 0;
		}
	}
	
} // end destructor


// Preform drawing operations. 
void MeshComponent::draw() const
{
	// A MeshComponent that is contained by a Gameobject that is
	// NOT active will not be rendered
	if (this->owningGameObject->getState() == ACTIVE) {

		// Render all subMeshes
		// Bind the shader program for the MeshComponent
		glUseProgram(shaderProgram);

		// Fetch and set the modeling transformation
		// Modeling matrix is specific to each object in the scene
		// Representing its pos, rot, and scale in the world
		mat4 modelingTransformation = this->owningGameObject->getModelingTransformation();
		SharedTransformations::setModelingMatrix(modelingTransformation);

		// Render all subMeshes in this MeshComponent
		for (auto& subMesh : subMeshes) {
			// Bind vertex array object for the subMesh
			glBindVertexArray(subMesh.vao);

			// Set the Materials properties in the uniform block buffer
			SharedMaterials::setShaderMaterialProperties(subMesh.material);
			if (subMesh.renderMode == ORDERED) 
			{
				// Trigger vertex fetch for ordered rendering 
				glDrawArrays(subMesh.primitiveMode, 0, subMesh.count);
			}
			else if (subMesh.renderMode == INDEXED) 
			{
				glDrawElements(subMesh.primitiveMode, subMesh.count, GL_UNSIGNED_INT, 0);
			}
		}

	}

} // end draw


void MeshComponent::addMeshComp(std::shared_ptr<MeshComponent> meshComponent)
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), meshComponent);

	if (iter == meshComps.end()) {

		meshComponent->buildMesh();

		meshComps.emplace_back(meshComponent);
		std::sort(meshComps.begin(), meshComps.end(), Component::CompareUpdateOrder);
	}

} // end addMeshComp

void MeshComponent::removeMeshComp(std::shared_ptr<MeshComponent> meshComponent)
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), meshComponent);

	if (iter != meshComps.end()) {

		if (VERBOSE) cout << "removeMeshComp" << endl;

		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, meshComps.end() - 1);
		meshComps.pop_back();
	}

} // end removeMeshComp

const std::vector<std::shared_ptr<MeshComponent>> & MeshComponent::GetMeshComponents()
{
	return meshComps;

}

SubMesh  MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData)
{
	// Create the SubMesh to be configured for the vertex data
	SubMesh subMesh;

	// Generate, bind, and load the vertex array object.
	// Store the identifier for the vertex array object in the subMesh
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	// Create a buffer and load the vertex positions and colors into it.
	// Store the identifier for the buffer in the subMesh.
	glGenBuffers(1, &subMesh.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, subMesh.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pntVertexData) * vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	// Specify the location and data format of an array of vertex 
	// positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	// Enable the flow of vertex positions into the vertex shader normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const void*)sizeof(glm::vec4));
	glEnableVertexAttribArray(1);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (GLvoid*)(7 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	// Store the number of vertices to be rendered in the subMesh
	subMesh.count = vertexData.size();

	// Store the renderMode in the subMesh for ORDERED rendering
	subMesh.renderMode = ORDERED;

	return subMesh;

} // end buildSubMesh

SubMesh MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData,
	const std::vector<unsigned int>& indices)
{
	// Create a submesh using the existing buildSubMesh method
	SubMesh subMesh = buildSubMesh(vertexData);

	// Generate and bind indices
	glGenBuffers(1, &subMesh.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexBuffer);
	// Copy the data into the buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	subMesh.renderMode = INDEXED;
	subMesh.count = indices.size();

	return subMesh;
}