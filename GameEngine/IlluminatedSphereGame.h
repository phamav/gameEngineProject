#pragma once
#include "GameEngine.h"
#include "SharedMaterials.h"

class IlluminatedSphereGame :
    public Game
{
protected:
	std::vector<std::shared_ptr<GameObject>> spheres;
	Material sphereMat1, sphereMat2;
	bool sphere_keyDown = false;
	unsigned int numSphere = 8;

	void loadScene() override
	{
		// BLUE COLOR
		ShaderInfo perFragShaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/PerFragShader.vs.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/PerFragShader.fs.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// RED COLOR
		ShaderInfo perVertexShaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/PerVertexShader.vs.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/PerVertexShader.fs.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// Build the shader programs
		GLuint perFragmentShaderProgram = BuildShaderProgram(perFragShaders);
		GLuint perVertexShaderProgram = BuildShaderProgram(perVertexShaders);

		// Allocate the buffer for the uniform variable values
		// and set up binding points
		SharedTransformations::setUniformBlockForShader(perFragmentShaderProgram);
		SharedTransformations::setUniformBlockForShader(perVertexShaderProgram);

		// Allocate the buffer for the material properties 
		// and set up binding points
		SharedMaterials::setUniformBlockForMaterial(perFragmentShaderProgram);
		SharedMaterials::setUniformBlockForMaterial(perVertexShaderProgram);

		// bind the shader program
		glUseProgram(perFragmentShaderProgram);
		sphereMat1.ambientMat = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		sphereMat1.diffuseMat = vec4(0.0f, 0.0f, 1.0f, 1.0f); // blue color for perfragment
		sphereMat1.specularMat = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		sphereMat1.emmissiveMat = vec4(0.0f, 0.0f, 0.1f, 1.0f);
		sphereMat1.specularExp = 32.0f;

		sphereMat2.ambientMat = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		sphereMat2.diffuseMat = vec4(1.0f, 0.0f, 0.0f, 1.0f); // red color for perVertex
		sphereMat2.specularMat = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		sphereMat2.emmissiveMat = vec4(0.0f, 0.0f, 0.1f, 1.0f);
		sphereMat2.specularExp = 32.0f;

		float startingX = -3.0f;
		float startingY = 1.0f;
		float gap = 2.0f;

		// build the spheres in two rows
		for (int row = 0; row < 2; row++)
		{
			GLuint currentShaderProgram;
			Material spheremat;

			if (row == 0) {
				spheremat = sphereMat1;
				currentShaderProgram = perFragmentShaderProgram;
			}
			else {
				glUseProgram(0);
				spheremat = sphereMat2;
				currentShaderProgram = perVertexShaderProgram;
				glUseProgram(perVertexShaderProgram);
			}

			// first row
			for (unsigned int col = 0; col < numSphere / 2; col++) {
				// create a spheremeshcomponent
				auto sphereObject = std::make_shared<GameObject>();
				this->addChildGameObject(sphereObject);

				auto sphereComp = std::make_shared<SphereMeshComponent>(currentShaderProgram, spheremat, 0.7f);
				sphereObject->addComponent(sphereComp);

				float xpos = startingX + col * gap;
				float ypos = startingY - row * gap;

				sphereObject->setPosition(vec3(xpos, ypos, -10.0f));

				spheres.push_back(sphereObject);
			}

			if (row == 1) glUseProgram(0);
		}

	} // end loadScene

};

