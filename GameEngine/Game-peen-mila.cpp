#include "GameEngine.h"
#include "BuildShaderProgram.h"
#include "MeshComponent.h"
#include "SharedTransformations.h"

#define VERBOSE false

//********************* Initialization Methods *****************************************

Game::Game(std::string windowTitle)
	: windowTitle(windowTitle)
{

} // end Game Constructor

void Game::runGame()
{
	// Set the owning game to this game. The will be passed to all
	// other GameObject when addChildGameObject is called
	this->owningGame = this;

	// The Game is the root of the scene graph. It has no parent.
	this->parent = nullptr;

	// Initialize the game
	bool success = initializeGame();

	// Enter the game loop
	if (success) {
		gameLoop();
	}

	// Free up resources
	shutdown();

} // end runGame

Game::~Game()
{
	if (VERBOSE) cout << "Game destructor " << endl;

} // end Game Destructor

bool Game::initializeGame()
{
	// Initialize the various libararies
	bool windowInit = initializeRenderWindow();
	bool graphicsInit = initializeGraphics();

	// Check if all libraries initialized correctly
	if (windowInit && graphicsInit)
	{
		// Set the owner of all the GameObject (temporary)
		GameObject::SetOwningGame(this);

		// Build the scene graph
		loadScene();

		// Explicitly call the resize method to set the initial projection transformation
		// and viewport based on framebuffer size.
		framebuffer_size_callback(this->renderWindow, initialScreenWidth, initialScreenHeight);

		this->initialize();

		// Return true to indicate the game has initialized
		return true;
	}
	else
	{
		std::cerr << "Game Initialization Failure." << endl;

		// Return false to indicate the game did not initialize correctly
		return false;
	}

} // end initialize

bool Game::initializeRenderWindow()
{
	// Initialize the GLFW and create a rendering window. If a failure, then exit
	if (!glfwInit()) {
		throw std::runtime_error("couldn't initialize GLFW");
	}

	// Register function to get GLFW error messages displayed on the console
	glfwSetErrorCallback(glfw_error_callback);
	// Specify that OpenGL will be the client API for the rendering context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	// Specify the OpenGL major and minor version and use the core OpenGL profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create window in debug mode for additional error and diagnostic reporting
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	// Flush the rendering pipeline when the context is released from being current
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
	// Explicitly request double buffers i.e. two frame buffers
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Create rendering window and OpenGL context
	renderWindow = glfwCreateWindow(initialScreenWidth, initialScreenHeight, windowTitle.c_str(),
		NULL, NULL);
	if (!renderWindow) {
		std::cerr << "Render Window Creation Failure." << endl;
		std::cerr << "Make sure requested OpenGL version is supported." << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(renderWindow);

	// Set the swap interval for the OpenGL context i.e. the number of screen
	// updates to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	bindCallBacks();
	if (VERBOSE) {
		cout << "Render Window Initialized" << endl;
	}

	return true;

} // end initializeRenderWindow

bool Game::initializeGraphics()
{
	// ****** Initialize GLEW *********
	
	// Prevent possible initialization error when using the core context
	glewExperimental = GL_TRUE;

	GLenum res = glewInit(); // Must be done after glut is initialized!
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	// On some platforms, GLEW will emit a benign error code. So clear it.
	glGetError();


	// ********* Initialize OpenGL **************
	// Register OpenGL error callback to print error
	// messages to the console
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglMessageCallback, 0);
	glDebugMessageCallback(openglMessageCallback, 0);
	
	// enabling depth testing, face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Render "filled in" polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Set source and destination blending functions
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glEnable(GL_BLEND); // turning blending on

	displayOpenGlInfo();

	// set clear color
	glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

	return true;

} // end initializeGraphics

void Game::bindCallBacks()
{
	// Set this game as the user defined window associated with the 
	// rendering window.
	glfwSetWindowUserPointer(renderWindow, this);

	// Register window resize callback function
	glfwSetFramebufferSizeCallback(renderWindow, framebufferResizeCallback);

	// Register window close function
	glfwSetWindowCloseCallback(renderWindow, windowCloseCallback);

} // end bindCallBacks


//********************* Run Methods *****************************************

void Game::gameLoop()
{
	isRunning = true;

	while (isRunning) {

		processGameInput();
		updateGame();
		renderScene();
	}

	if (VERBOSE) cout << "Exited Game Loop" << endl;

} // end gameLoop

void Game::processGameInput()
{
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_ESCAPE)) {

		isRunning = false;
	}

	// toggle wireframe mode on 'W' key press
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) && WireFrame_KeyDown == false) {

		GLint mode;
		glGetIntegerv(GL_POLYGON_MODE, &mode);

		if (mode == GL_FILL) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		WireFrame_KeyDown = true;

	}
	else if (!glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W)) {
		WireFrame_KeyDown = false;
	}

	// 11. Callback function: Print message for mouse enters/exits
	glfwSetCursorEnterCallback(renderWindow, mouseEnterExit);

	// 12. Print j everytime j key is pushed
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J) == GLFW_PRESS && jkeypressed == false) {
		cout << "j " << endl;
		jkeypressed = true;
	}
	else if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_J) == GLFW_RELEASE) {
		jkeypressed = false;
	}

	// 13. add callback everytime mouse is pressed / released
	glfwSetMouseButtonCallback(renderWindow, mouseButtonCallback);

	// 14. Custom callback function
	// Returns cursor's position on the rendering window.
	glfwSetCursorPosCallback(renderWindow, mousePosCallback);

	// Start an input traversal of all SceneGrapNode/GameObjects in the game
	GameObject::processInput();

	// Add pending, delete removed, and reparent
	GameObject::UpdateSceneGraph();

} // end processInput

void Game::updateGame()
{
	// Compute delta time
	static double lastRenderTime = glfwGetTime(); // static initilization only occurs once
	double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastRenderTime);

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (deltaTime >= FRAME_INTERVAL) {

		// Must be called in order for callback functions
		// to be called for registered events.
		glfwPollEvents();

		// Start an update traversal of all SceneGrapNode/GameObjects in the game
		GameObject::update(deltaTime);

		// Add pending, delete removed, and reparent
		GameObject::UpdateSceneGraph();

		// Update the last time the game was updated
		lastRenderTime = currentTime;
	}

} // end updateGame()

// gets called from gameLoop()
void Game::renderScene()
{
	// calculate the view matrix
	mat4 viewTrans = glm::translate(vec3(0.0f, 0.0f, 0.0f));
	SharedTransformations::setViewMatrix(viewTrans);

	// Clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render loop
	// cout << "render the scene" << endl;

	// iterate through all the MeshComponents that have been
	// added to the game and call draw method for each one.
	for (auto& mesh : MeshComponent::GetMeshComponents())
	{
		mesh->draw();
	};

	// Swap the front and back buffers
	glfwSwapBuffers(renderWindow);

} // end renderScene

//********************* Shutdown Methods *****************************************

void Game::shutdown()
{
	// Destroy the window
	glfwDestroyWindow(renderWindow);

	// Frees other glfw allocated resources
	glfwTerminate();

	deleteAllShaderPrograms();

} // end shutDown

//********************* Accessor Methods *****************************************

glm::ivec2 Game::getWindowDimensions()
{
	int width, height;
	glfwGetFramebufferSize(this->renderWindow, &width, &height);

	return glm::ivec2(width, height);

} // end getWindowDimensions

//********************* Event Handlers *****************************************

void Game::window_close_callback(GLFWwindow* window)
{
	cout << "Window closed." << endl;
	// Stop the game loop
	this->isRunning = false;

} // end window_close_callback


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	cout << "Window resized to " << width << " x " << height << " pixels." << endl;
	
	// initialize window rendering view
	glViewport(0, 0, width, height);

	// calculate aspect ratio
	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	// Set the projection transformation ...
	// projMat typically only needs to be set or updated when
	// the window is resized of if the camera's properties change
	mat4 projMat = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f);
	SharedTransformations::setProjectionMatrix(projMat);

	// set the modeling matrix
} // end framebuffer_size_callback

//********************* static function definitions *****************************************

void glfw_error_callback(int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);

} // end error_callback


void displayOpenGlInfo(void)
{
	// Display the company responsible for this GL implementation
	fprintf(stdout, "OpenGL INFO:\n\tOpenGL Implementor: %s\n",
		glGetString(GL_VENDOR));
	// Display the renderer/graphics card
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	// Display the OpenGL version that is implemented
	fprintf(stdout, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	// Display the GLSL version
	fprintf(stdout, "\tGLSL Version: %s\n\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));

} // end displayOpenGlInfo


void GLAPIENTRY openglMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	if (severity != 0x826B) {
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

} // end MessageCallback

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));
	that->framebuffer_size_callback(window, width, height);
}

static void windowCloseCallback(GLFWwindow* window)
{
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));
	that->window_close_callback(window);
}

static void mouseEnterExit(GLFWwindow* window, int entered) {
	if (entered) cout << "Mouse entered the rendering window." << endl;
	else cout << "Mouse exited the rendering window." << endl;
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) cout << "Right mouse button pressed." << endl;
		else if (action == GLFW_RELEASE) cout << "Right mouse button released." << endl;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) cout << "Left mouse button pressed." << endl;
		else if (action == GLFW_RELEASE) cout << "Left mouse button released." << endl;
	}
}

static void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
	cout << "Mouse position: (" << xpos << ", " << ypos << ")" << endl;
}
