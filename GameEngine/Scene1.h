#pragma once

#include "GameEngine.h"

class Scene1 : public Game
{
protected:
	std::shared_ptr<GameObject> gameObject;
	std::shared_ptr<GameObject> gameObject1;
	std::shared_ptr<GameObject> gameObject2;
	std::shared_ptr<GameObject> gameObject3;
	std::shared_ptr<GameObject> gameObject4;

	bool zero_keyDown = false;
	bool one_keyDown = false;
	bool two_keyDown = false;
	bool three_keyDown = false;
	bool four_keyDown = false;

	// Turn rendering of triangle on and off 
	// using '0' key to toggle state of GameObject
	// that contains TriangleMeshComponent
	virtual void processGameInput()
	{
		Game::processGameInput(); // call to superclass


		// toggle triangle mode on '0' key press
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_0) && zero_keyDown == false) 
		{
			if (gameObject->getState() == ACTIVE) {
				gameObject->setState(PAUSED);
			}
			else if(gameObject->getState() == PAUSED) {
				gameObject->setState(ACTIVE);
			}
			zero_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_0)) {
			zero_keyDown = false;
		}

		// 1 mode
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1) && !one_keyDown)
		{
			if (gameObject1->getState() == ACTIVE) {
				gameObject1->setState(PAUSED);
			}
			else {
				gameObject1->setState(ACTIVE);
			}
			one_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_1)) {
			one_keyDown = false;
		}

		// 2 mode
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_2) && !two_keyDown)
		{
			cout << "2nd key pressed" << endl;
			if (gameObject2->getState() == ACTIVE) {
				gameObject2->setState(PAUSED);
			}
			else {
				gameObject2->setState(ACTIVE);
			}
			two_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_2)) {
			two_keyDown = false;
		}

		// 3 mode
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_3) && !three_keyDown)
		{
			cout << "3rd key pressed" << endl;
			if (gameObject3->getState() == ACTIVE) {
				gameObject3->setState(PAUSED);
			}
			else {
				gameObject3->setState(ACTIVE);
			}
			three_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_3)) {
			three_keyDown = false;
		}

		// 4 mode
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_4) && !four_keyDown)
		{
			cout << "4th key pressed" << endl;
			if (gameObject4->getState() == ACTIVE) {
				gameObject4->setState(PAUSED);
			}
			else {
				gameObject4->setState(ACTIVE);
			}
			four_keyDown = true;
		}
		else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_4)) {
			four_keyDown = false;
		}
	}

	void loadScene() override
	{
		// Build shader program
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "Shaders/vertexShader.glsl" },
			{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.glsl" },
			{ GL_NONE, NULL } // signals that there are no more shaders 
		};

		// pass the array to BuildShaderProgram to build the shader program
		GLuint shaderProgram = BuildShaderProgram(shaders);

		// Allocate the buffer for the uniform variable values
		// and set up binding points
		SharedTransformations::setUniformBlockForShader(shaderProgram);

		// bind the shader program
		glUseProgram(shaderProgram);

		// instantiate a TriangleMeshComponent object
		// and assign the address to a pointer
		gameObject = std::make_shared<GameObject>();
		this->addChildGameObject(gameObject);
		std::shared_ptr<TriangleMeshComponent> triangle =
			std::make_shared<TriangleMeshComponent>(shaderProgram);
		gameObject->addComponent(triangle);

		// figure 1
		gameObject1 = std::make_shared<GameObject>();
		this->addChildGameObject(gameObject1);
		std::shared_ptr<Figure1> fig1 =
			std::make_shared<Figure1>(shaderProgram);
		gameObject1->addComponent(fig1);
		gameObject1->setState(PAUSED);

		// figure 2
		gameObject2 = std::make_shared<GameObject>();
		this->addChildGameObject(gameObject2);
		std::shared_ptr<Figure2> fig2 =
			std::make_shared<Figure2>(shaderProgram);
		gameObject2->addComponent(fig2);
		gameObject2->setState(PAUSED);

		// figure 3
		gameObject3 = std::make_shared<GameObject>();
		this->addChildGameObject(gameObject3);
		std::shared_ptr<Figure3> fig3 =
			std::make_shared<Figure3>(shaderProgram);
		gameObject3->addComponent(fig3);
		gameObject3->setState(PAUSED);

		// Create a SphereMeshComponent
		gameObject4 = std::make_shared<GameObject>();
		this->addChildGameObject(gameObject4);
		Material sphereMat;
		//sphereMat.basicColor = BLUE_RGBA;
		std::shared_ptr<SphereMeshComponent> sphere =
			std::make_shared<SphereMeshComponent>(shaderProgram, sphereMat, 0.5f);
		gameObject4->addComponent(sphere);
		gameObject4->setState(PAUSED);

	} // end loadScene
	
};
