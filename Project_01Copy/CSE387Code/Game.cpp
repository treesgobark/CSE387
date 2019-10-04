#include "Game.h"
#include <algorithm>

#include "GameObject.h"
#include "Component.h"
#include "Mesh.h"
#include "ModelMesh.h"

#include "SharedGeneralLighting.h"
#include "SharedMaterialProperties.h"
#include "SharedProjectionAndViewing.h"
#include "BuildShaderProgram.h"

const int initialScreenWidth = 1024;
const int initialScreenHeight = 768;

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLdouble FRAME_INTERVAL = 1.0 / FRAMES_PER_SECOND; // Interval in milliseconds between frames
float deltaTime = 0.0;

//********************* Static Function declarations *****************************************

void error_callback(int error, const char* description);
void displayOpenGlInfo(void);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, 
								GLsizei length, const GLchar* message, const void* userParam);

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

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game* that = static_cast<Game*>(glfwGetWindowUserPointer(window));
	that->key_callback( window, key, scancode, action, mods);
}

//********************* Initialization Methods *****************************************

ModelMesh* dino1;
GLuint shaderProgram;

Game::Game(std::string windowTitle)
	: isRunning(true), updatingGameObjects(false), windowTitle(windowTitle)
{
}

Game::~Game()
{
}

bool Game::initialize()
{

	bool windowInit = initializeRenderWindow();

	bool graphicsInit = initializeGraphics();

	if (windowInit && graphicsInit) {

		loadData();

		InitializeGameObjects();

		framebuffer_size_callback(this->renderWindow, initialScreenWidth, initialScreenHeight);

	}
	else {

		return false;
	}

}


bool Game::initializeRenderWindow()
{
	glfwInit();

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Use the core OpenGL profile	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create rendering window and the OpenGL context.
	this->renderWindow = glfwCreateWindow(initialScreenWidth, initialScreenHeight, this->windowTitle.c_str(), NULL, NULL);

	if (!renderWindow) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(renderWindow);

	// Set the swap interval for the OpenGL context i.e. the number of screen updates 
	// to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	cout << "Render Window Initialized" << endl;

	// Prevent possible initialization error when using the core context
	glewExperimental = GL_TRUE;

	GLenum res = glewInit(); // Must be done after glut is initialized!

	if (res != GLEW_OK) {

		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	// On some platforms, GLEW will emit a benign error code. So clear it.
	glGetError();

	bindCallBacks();

	return true;

} // end initializeRenderWindow


bool Game::initializeGraphics()
{
	// Display OpenGL context information (OpenGL and GLSL versions) on the
	// command console.
	displayOpenGlInfo();

	// Register OpenGL error callback to print error messages to the console
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn on depth testing

	// Cull back faces of polygons
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	cout << "Graphics Initialized" << endl;

	return true;

} // end initializeGraphics


 void Game::loadData()
{
	 glClearColor(static_cast<GLclampf>(0.2), static_cast<GLclampf>(0.5), static_cast<GLclampf>(0.8), static_cast<GLclampf>(1.0));

	 // Variable to hold the integer identifier for the shader program
	// GLuint shaderProgram;

	 // Build shader program
	 ShaderInfo shaders[] = {
		 { GL_VERTEX_SHADER, "Shaders/vertexShader.vs.glsl" },
		 { GL_FRAGMENT_SHADER, "Shaders/fragmentShader.fs.glsl" },
		 { GL_NONE, NULL } // signals that there are no more shaders 
	 };

	 shaderProgram = BuildShaderProgram(shaders);

	 glUseProgram(shaderProgram);

	 SharedProjectionAndViewing::setUniformBlockForShader(shaderProgram);
	 SharedMaterialProperties::setUniformBlockForShader(shaderProgram);
	 SharedGeneralLighting::setUniformBlockForShader(shaderProgram);

	 SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	 SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 0.0f));
	 SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, true);

	 dino1 = new ModelMesh("Assets/Dinosaur/Trex.obj");

	 cout << "Data Initialized" << endl;

} // end loadData


void Game::InitializeGameObjects()
{

	dino1->initialize(shaderProgram);

	dino1->modelMatrix = glm::rotate(glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));

	cout << "Components Initialized" << endl;
}


void Game::bindCallBacks()
{
	glfwSetWindowUserPointer(renderWindow, this);
	glfwSetFramebufferSizeCallback(renderWindow, framebufferResizeCallback);
	glfwSetWindowCloseCallback(renderWindow, windowCloseCallback);
	glfwSetKeyCallback(renderWindow, keyCallback);
	glfwSetErrorCallback(error_callback);
}

//********************* Run Methods *****************************************

void Game::gameLoop()
{
	while (isRunning && !glfwWindowShouldClose(renderWindow)) {
		processInput();
		updateGame();
		renderScene();
	}
}

void Game::processInput()
{
	glfwPollEvents();
}
void Game::updateGame()
{
	// Compute delta time
	static double lastRenderTime = glfwGetTime();
	double currentTime = glfwGetTime();
	deltaTime = static_cast<float>(currentTime - lastRenderTime);

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (deltaTime >= FRAME_INTERVAL) {

		// Update all gameObjects
		updatingGameObjects = true;

		for (auto thisGameObject : inGameObjects) {
			thisGameObject->Update(deltaTime);
		}

		updatingGameObjects = false;

		// Move any pending GameObject to inGameObjects
		for (auto pending : pendingGameObjects) {

			inGameObjects.emplace_back(pending);
		}
		pendingGameObjects.clear();

		// Add any dead GameObjects to a temp vector
		std::vector<GameObject*> deadGameObjects;

		for (auto gameObject : inGameObjects) {
			if (gameObject->getState() == GameObject::EDead) {
				deadGameObjects.emplace_back(gameObject);
			}
		}

		// Delete dead game (which removes them from inGameObjects)
		for (auto gameObject : deadGameObjects) {
			delete gameObject;
		}
		deadGameObjects.clear();

		// Temp
		// Set the viewing transformation
		mat4 viewingTransformation = glm::lookAt(vec3(0.0f, 5.0f, 20.0f), vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewingTransformation));
		SharedProjectionAndViewing::setViewMatrix(viewingTransformation);

		// Save current time to determine when the scene should be rendered next
		lastRenderTime = currentTime;
	}

} // end updateGame()

void Game::renderScene()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dino1->draw();

	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(renderWindow);

} // end renderScene


void Game::addGameObject(GameObject* actor)
{
	// If updating gameObjects, need to add to pending
	if (updatingGameObjects) {
		pendingGameObjects.emplace_back(actor);
	}
	else {
		inGameObjects.emplace_back(actor);
	}

} // end addGameObject

void Game::removeGameObject(GameObject* actor)
{
	// Is it in pending gameObjects?
	auto iter = std::find(pendingGameObjects.begin(), pendingGameObjects.end(), actor);

	if (iter != pendingGameObjects.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, pendingGameObjects.end() - 1);
		pendingGameObjects.pop_back();
	}

	// Is it in inGameObjects?
	iter = std::find(inGameObjects.begin(), inGameObjects.end(), actor);

	if (iter != inGameObjects.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, inGameObjects.end() - 1);
		inGameObjects.pop_back();
	}

} // end removeGameObject

GameObject* Game::findGameObjectByName(string name)
{
	for (auto i : inGameObjects) {
		if (i->getName() == name) {
			return i;
		}
	}
	for (auto i : pendingGameObjects) {
		if (i->getName() == name) {
			return i;
		}
	}
	return nullptr;
}

void Game::AddMeshComp(Mesh* mesh)
{
	meshComps.emplace_back(mesh);
}

void Game::RemoveMeshComp(Mesh* mesh)
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
	meshComps.erase(iter);
}

//********************* Shutdown Methods *****************************************

void Game::unloadData()
{
	// Delete gameObjects
	
	delete dino1;

	// Because ~GameObject calls RemoveGameObject, have to use a different style loop
	while (!inGameObjects.empty()) {

		delete inGameObjects.back();
	}

	while (!pendingGameObjects.empty()) {

		delete pendingGameObjects.back();
	}

} // end unloadData



void Game::shutdown()
{
	// Frees all remaining resources allocated by GLFW
	glfwTerminate();

	unloadData();


}

//********************* Event Handlers *****************************************

void Game::window_close_callback(GLFWwindow* window) {

	// Destroy the window
	glfwDestroyWindow(window);

} // end window_close_callback


void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

	// Set the modeling transformation
	glm::mat4 projectionTransformation = glm::perspective(PI / 4.0f, ((float)width) / height, 0.5f, 1000.0f);
	//glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransformation));
	SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);

} // end framebuffer_size_callback


void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {

		switch (key) {

		case GLFW_KEY_ESCAPE:

			glfwSetWindowShouldClose(window, GL_TRUE);

			break;

		default:

			fprintf(stdout, "key pressed\n");
		}

	}

} // end key_callback


//********************* static function definitions *****************************************

void error_callback(int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);

} // end error_callback

/**
Function: Displays OpengGL and GLSL version information.
*/
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

void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
								 const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);

} // end MessageCallback