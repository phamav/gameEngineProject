#pragma once
#include "GameEngine.h"

class IlluminatedSphereGame :
    public Game
{
protected:
	std::vector<std::shared_ptr<GameObject>> spheres;
	bool sphere_keyDown = false;
	unsigned int numSphere = 8;

	// Turn rendering of triangle on and off 
	// using '0' key to toggle state of GameObject
	// that contains TriangleMeshComponent
	virtual void processGameInput()
	{
		Game::processGameInput(); // call to superclass


		// toggle triangle mode on '0' key press to render spheres
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_0) && sphere_keyDown == false)
		{
			for (auto& sphere : spheres)
			{
				if (sphere->getState() == ACTIVE) {
					sphere->setState(PAUSED);
				}
				else if (sphere->getState() == PAUSED) {
					sphere->setState(ACTIVE);
				}
			}
			sphere_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_0)) {
			sphere_keyDown = false;
		}
		
	}

	void loadScene() override
	{
		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/perFragShader.vs.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/perFragShader.fs.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// pass the array to BuildShaderProgram to build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		// Allocate the buffer for the uniform variable values
		// and set up binding points
		SharedTransformations::setUniformBlockForShader(shaderProgram);

		// bind the shader program
		glUseProgram(shaderProgram);

		float startingX = -3.0f;
		float startingY = 1.0f;
		float gap = 2.0f;

		// build the spheres in two rows
		for (int row = 0; row < 2; row++)
		{
			// first row
			for (unsigned int col = 0; col < numSphere / 2; col++) {
				// Create a SphereMeshComponent
				auto sphereObject = std::make_shared<GameObject>();
				this->addChildGameObject(sphereObject);

				Material sphereMat;
				sphereMat.basicColor = BLUE_RGBA;


				auto sphereComp = std::make_shared<SphereMeshComponent>(shaderProgram, sphereMat, 0.5f);
				sphereObject->addComponent(sphereComp);
				sphereObject->setState(PAUSED);

				float xPos = startingX + col * gap;
				float yPos = startingY - row * gap;

				sphereObject->setPosition(vec3(xPos, yPos, -10.0f));

				spheres.push_back(sphereObject);
			}

		}


	} // end loadScene

};

