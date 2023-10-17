#include "SphereMeshComponent.h"

vec4 sphericalToCartesian(float sliceAngle, float stackAngle,
	float radius)
{
	vec4 v(glm::cos(stackAngle) * glm::sin(sliceAngle) * radius,
		glm::sin(stackAngle) * radius,
		glm::cos(stackAngle) * cos(sliceAngle) * radius,
		1.0f);

	return v;

} // end sphericalToCartesian

// Get the Spherical texture mapping coordinates based on slice and 
// stack angles
glm::vec2 getSphericalTextCoords(float sliceAngle, float stackAngle)
{
	float s = sliceAngle / (2.0f * PI);
	float t = (stackAngle + PI / 2) / PI;

	return glm::vec2(s, t);
}

SphereMeshComponent::SphereMeshComponent(GLuint shaderProgram, Material mat, float radius, int stacks, int slices, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder), sphereMat(mat), radius(radius), stacks(stacks), slices(slices)
{
	sliceInc = (2.0f * PI) / slices;
	stackInc = PI / stacks;

}

void SphereMeshComponent::buildMesh()
{
		initializeBottomSubMesh();

		initializeBodySubMesh();

		initializeTopSubMesh();

} // end bulidMesh


void SphereMeshComponent::initializeBottomSubMesh()
{
	std::vector<pntVertexData> pnt;

	lower = createStackVertexData(sliceInc / 2.0f, -PI_OVER_2);

	upper = createStackVertexData(0.0f, -PI_OVER_2 + stackInc);

	for (int j = slices; j > 0; j--) {

		pnt.push_back(lower[(j - 1)]);
		pnt.push_back(upper[j]);
		pnt.push_back(upper[(j - 1)]);
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

}// end initializeBottomSubMesh


void SphereMeshComponent::initializeBodySubMesh()
{
	std::vector<pntVertexData> pnt;
	std::vector<unsigned int> indices;

	float stackAngle = -PI_OVER_2 + stackInc;

	lower = upper; // Assumes initializeBottomSubMesh was previously called

	lower = createStackVertexData(0.0f, stackAngle);

	for (int j = 0; j < stacks - 2; j++) {

		stackAngle += stackInc;

		upper = createStackVertexData(0.0f, stackAngle);

		for (int i = 0; i < slices; i++) {

			pnt.push_back(upper[i]);
			pnt.push_back(lower[i]);
			pnt.push_back(lower[(i + 1)]);

			pnt.push_back(upper[i]);
			pnt.push_back(lower[(i + 1)]);
			pnt.push_back(upper[(i + 1)]);
		}

		lower = upper;
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	subMeshes.push_back(subMesh);

} // end initializeBodySubMesh


void SphereMeshComponent::initializeTopSubMesh()
{
	std::vector<pntVertexData> pnt;
	std::vector<unsigned int> indices;

	lower = upper; // Assumes initializeBodySubMesh was previously called

	upper = createStackVertexData(sliceInc / 2.0f, PI_OVER_2);

	lower = createStackVertexData(0.0f, PI_OVER_2 - stackInc);

	for (int j = 0; j < slices; j++) {

		pnt.push_back(upper[j]);
		pnt.push_back(lower[j]);
		pnt.push_back(lower[(j+1)]);
	}

	SubMesh subMesh = buildSubMesh(pnt);

	// Set the primitive mode for the vertex data
	subMesh.primitiveMode = GL_TRIANGLES;

	subMesh.material = sphereMat;

	// Push the submesh into vector of Submeshes to be rendered
	this->subMeshes.push_back(subMesh);

} // end initializeTopSubMesh


std::vector<pntVertexData> SphereMeshComponent::createStackVertexData(float startingSliceAngle, float stackAngle)
{
	std::vector<pntVertexData> stackVertexData;

	float sliceAngle = startingSliceAngle;

	for (int i = 0; i <= slices; i++) {

		vec4 v0 = sphericalToCartesian(sliceAngle, stackAngle, radius);
		vec2 t0 = getSphericalTextCoords(sliceAngle, stackAngle);

		stackVertexData.push_back( pntVertexData(v0, glm::normalize(v0).xyz, t0) );

		sliceAngle += sliceInc;
	}

	return stackVertexData;
}

