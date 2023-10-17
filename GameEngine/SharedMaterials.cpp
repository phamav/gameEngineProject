#include "SharedMaterials.h"

// Modeling this block for shader
//layout(shared) uniform materialBlock
//{
//    vec4 ambientMat;
//    vec4 diffuseMat;
//    vec4 specularMat;
//    vec4 emmissiveMat;
//    float specularExp;
//};

GLuint SharedMaterials::ambientLocation;
GLuint SharedMaterials::diffuseLocation;
GLuint SharedMaterials::specularLocation;
GLuint SharedMaterials::emmissiveLocation;
GLuint SharedMaterials::specularExpLocation;

// current data held in buffer
vec4 SharedMaterials::ambientMat;
vec4 SharedMaterials::diffuseMat;
vec4 SharedMaterials::specularMat;
vec4 SharedMaterials::emmissiveMat;
float SharedMaterials::specularExp;

SharedUniformBlock SharedMaterials::materialBlock(materialBlockBindingPoint);

const std::string SharedMaterials::materialBlockName = "materialBlock";

void SharedMaterials::setUniformBlockForMaterial(GLuint shaderProgram) {
	std::vector<std::string> matBlockMemberNames = { "ambientMat", "diffuseMat", "specularMat", "emmissiveMat", "specularExp"};

	std::vector<GLint> uniformOffsets = materialBlock.setUniformBlockForShader(shaderProgram, materialBlockName, matBlockMemberNames);

	// Save locations
	ambientLocation = uniformOffsets[0];
	diffuseLocation = uniformOffsets[1];
	specularLocation = uniformOffsets[2];
	emmissiveLocation = uniformOffsets[3];
	specularExpLocation = uniformOffsets[4];

	uniformOffsets.clear();
}

void SharedMaterials::setShaderMaterialProperties(const Material& material) {
	if (materialBlock.getSize() > 0) {
		// bind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, materialBlock.getBuffer());

		SharedMaterials::ambientMat = material.ambientMat;
		glBufferSubData(GL_UNIFORM_BUFFER, ambientLocation, sizeof(glm::vec4), glm::value_ptr(ambientMat));

		SharedMaterials::diffuseMat = material.diffuseMat;
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseLocation, sizeof(glm::vec4), glm::value_ptr(diffuseMat));

		SharedMaterials::specularMat = material.specularMat;
		glBufferSubData(GL_UNIFORM_BUFFER, specularLocation, sizeof(glm::vec4), glm::value_ptr(specularMat));

		SharedMaterials::emmissiveMat = material.emmissiveMat;
		glBufferSubData(GL_UNIFORM_BUFFER, emmissiveLocation, sizeof(glm::vec4), glm::value_ptr(emmissiveMat));

		SharedMaterials::specularExp = material.specularExp;
		glBufferSubData(GL_UNIFORM_BUFFER, specularExpLocation, sizeof(float), &specularExp);

		// unbind the buffer
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

