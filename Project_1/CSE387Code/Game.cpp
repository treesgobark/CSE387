#include "Game.h"
#include <algorithm>

#include "GameObject.h"
#include "Component.h"
#include "MeshComponent.h"
#include "ModelMeshComponent.h"

#include "SharedGeneralLighting.h"
#include "SharedMaterialProperties.h"
#include "SharedProjectionAndViewing.h"
#include "BuildShaderProgram.h"

#include "SimpleMoveComponent.h"
#include "SoundEngine.h"


//********************* Static Function declarations *****************************************

void glfw_error_callback(int error, const char* description);
void displayOpenGlInfo(void);
void GLAPIENTRY openglMessageCallback(	GLenum source, GLenum type, GLuint id, GLenum severity, 
										GLsizei length, const GLchar* message, const void* userParam);
SoundSourceComponent* roar;

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
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_1:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, !SharedGeneralLighting::getEnabled(GL_LIGHT_ZERO));
			break;
		case GLFW_KEY_2:
			SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, !SharedGeneralLighting::getEnabled(GL_LIGHT_ONE));
			break;
		case GLFW_KEY_3:
			SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, !SharedGeneralLighting::getEnabled(GL_LIGHT_TWO));
			break;
		case GLFW_KEY_4:
			SharedGeneralLighting::setEnabled(GL_LIGHT_THREE, !SharedGeneralLighting::getEnabled(GL_LIGHT_THREE));
			break;
		}
	}
	that->key_callback( window, key, scancode, action, mods);
}

//********************* Initialization Methods *****************************************


Game::Game(std::string windowTitle)
	: windowTitle(windowTitle)
{

} // end Game Constructor

Game::~Game()
{
	SoundEngine::Stop();
} // end Game Destructor

bool Game::initialize()
{
	bool windowInit = initializeRenderWindow();

	bool graphicsInit = initializeGraphics();

	if (windowInit && graphicsInit) {

		loadData();

		initializeGameObjects();

		framebuffer_size_callback(this->renderWindow, initialScreenWidth, initialScreenHeight);

		return true;
	}
	else {

		return false;
	}

} // end initialize


bool Game::initializeRenderWindow()
{
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Use the core OpenGL profile	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// See https://www.glfw.org/docs/latest/window_guide.html#window_hints for
	// more information on window hints and default values
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_FLUSH);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Create rendering window and the OpenGL context.
	this->renderWindow = glfwCreateWindow(initialScreenWidth, initialScreenHeight, this->windowTitle.c_str(), NULL, NULL);

	if (!renderWindow) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(renderWindow);

	// When sticky keys mode is enabled, the pollable state of a key will remain 
	// GLFW_PRESS until the state of that key is polled with glfwGetKey. Once it 
	// has been polled, if a key release event had been processed in the meantime,
	// the state will reset to GLFW_RELEASE, otherwise it will remain GLFW_PRESS.
	glfwSetInputMode(renderWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

	// Set the swap interval for the OpenGL context i.e. the number of screen updates 
	// to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	cout << "Render Window Initialized" << endl;

	// gl3w is a replacement for GLEW
	if (gl3wInit()) {
		printf("failed to initialize OpenGL\n");
		return false;
	}

	bindCallBacks();

	return true;

} // end initializeRenderWindow


bool Game::initializeGraphics()
{
	// Display OpenGL context information (OpenGL and GLSL versions) on the
	// command console.
	displayOpenGlInfo();

	// Register OpenGL error callback to print error messages to the console
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(openglMessageCallback, 0);

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


	 // Build shader program
	 ShaderInfo shaders[] = {
		 { GL_VERTEX_SHADER, "Shaders/vertexShader.vs.glsl" },
		 { GL_FRAGMENT_SHADER, "Shaders/fragmentShader.fs.glsl" },
		 { GL_NONE, NULL } // signals that there are no more shaders 
	 };

	 GLuint shaderProgram = BuildShaderProgram(shaders);

	 glUseProgram(shaderProgram);

	 SharedProjectionAndViewing::setUniformBlockForShader(shaderProgram);
	 SharedMaterialProperties::setUniformBlockForShader(shaderProgram);
	 SharedGeneralLighting::setUniformBlockForShader(shaderProgram);

	 SharedGeneralLighting::setAmbientColor(GL_LIGHT_ZERO, vec4(1.0, 1.0, 1.0, 1.0));
	 SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, true);

	 SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ONE, vec4(0.8, 0.3, 0.3, 1.0));
	 SharedGeneralLighting::setSpecularColor(GL_LIGHT_ONE, vec4(0.8, 0.3, 0.3, 1.0));
	 SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ONE, vec4(0.5, 0.5, 0.5, 1.0));
	 SharedGeneralLighting::setEnabled(GL_LIGHT_ONE, true);

	 SharedGeneralLighting::setDiffuseColor(GL_LIGHT_TWO, vec4(0.3, 0.8, 0.3, 1.0));
	 SharedGeneralLighting::setSpecularColor(GL_LIGHT_TWO, vec4(0.3, 0.8, 0.3, 1.0));
	 SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_TWO, normalize(vec4(-1.0, -1.0, -1.0, 0.0)));
	 SharedGeneralLighting::setEnabled(GL_LIGHT_TWO, true);

	 SharedGeneralLighting::setDiffuseColor(GL_LIGHT_THREE, vec4(0.3, 0.3, 0.8, 1.0));
	 SharedGeneralLighting::setSpecularColor(GL_LIGHT_THREE, vec4(0.3, 0.3, 0.8, 1.0));
	 SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_THREE, vec4(0.0, 2.0, 0.0, 1.0));
	 SharedGeneralLighting::setIsSpot(GL_LIGHT_THREE, true);
	 SharedGeneralLighting::setSpotDirection(GL_LIGHT_THREE, vec4(0.0, 0.0, -1.0, 1.0));
	 SharedGeneralLighting::setSpotCutoffCos(GL_LIGHT_THREE, 0.8);
	 SharedGeneralLighting::setEnabled(GL_LIGHT_THREE, true);

	 GameObject* emptyGameObject = new GameObject(this);
	 this->sceneNode.addChild(emptyGameObject);
	 emptyGameObject->sceneNode.setPosition(vec3(0.0f, 0.0f, 0.0f), LOCAL);

	 emptyGameObject->addComponent();

	 GameObject* dinoGameObject = new GameObject(this);
	 dinoGameObject->addComponent(new ModelMeshComponent("Assets/Dinosaur/Trex.obj", shaderProgram));
	 dinoGameObject->addComponent(new SimpleMoveComponent());

	 roar = new SoundSourceComponent("Assets/T_Goose.mp3", 4);
	 dinoGameObject->addComponent(roar);

	 GameObject* oliveGameObject = new GameObject(this);
	 oliveGameObject->localTransform = scale(oliveGameObject->localTransform * translate(vec3(-2.0f, 0.0f, -5.0f)), vec3(0.1));
	 oliveGameObject->addComponent(new ModelMeshComponent("Assets/OliveOil/olive_oil.obj", shaderProgram));
	 oliveGameObject->addComponent(new SimpleMoveComponent());

	 GameObject* antGameObject = new GameObject(this);
	 antGameObject->addComponent(new ModelMeshComponent("Assets/Ant/ant.obj", shaderProgram));
	 antGameObject->addComponent(new SimpleMoveComponent());

} // end loadData


void Game::initializeGameObjects()
{
	for (auto gameObject : inGameObjects) {

		gameObject->initialize();
	}

} // end initializeGameObjects


void Game::bindCallBacks()
{
	glfwSetWindowUserPointer(renderWindow, this);
	glfwSetFramebufferSizeCallback(renderWindow, framebufferResizeCallback);
	glfwSetWindowCloseCallback(renderWindow, windowCloseCallback);
	glfwSetKeyCallback(renderWindow, keyCallback);
	glfwSetErrorCallback(glfw_error_callback);

} // end bindCallBacks

//********************* Run Methods *****************************************

void Game::gameLoop()
{
	while (isRunning) {
		processInput();
		updateGame();
		renderScene();
	}

	shutdown();

} // end gameLoop

/*
https://www.glfw.org/docs/latest/input_guide.html
https://www.glfw.org/docs/latest/group__input.html
https://www.glfw.org/docs/latest/group__keys.html

*/
void Game::processInput()
{
	this->updatingGameObjects = true;

	// Update all the game objects in the game
	for (auto gameObject : this->inGameObjects) {
		gameObject->processInput();
	}
	this->updatingGameObjects = false;

	// Must be called in order for callback functions
	// to be called for registered events.
	glfwPollEvents();

}  //  end processInput


void Game::updateGame()
{
	// Compute delta time
	static double lastRenderTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = static_cast<float>(currentTime - lastRenderTime);

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (deltaTime >= FRAME_INTERVAL) {

		// Update all gameObjects
		this->updatingGameObjects = true;

		for (auto thisGameObject : this->inGameObjects) {
			thisGameObject->update(deltaTime);
		}

		this->updatingGameObjects = false;

		// Move any pending GameObject to this->inGameObjects
		for (auto pending : pendingGameObjects) {

			// Initialize the pending GameObject and add it to the game
			pending->initialize();

			this->inGameObjects.emplace_back(pending);
		}
		pendingGameObjects.clear();

		// Add any dead GameObjects to a temp vector
		std::vector<GameObject*> deadGameObjects;

		for (auto gameObject : this->inGameObjects) {
			if (gameObject->getState() == GameObject::DEAD) {
				deadGameObjects.emplace_back(gameObject);
			}
		}

		// Delete dead game (which removes them from this->inGameObjects)
		for (auto gameObject : deadGameObjects) {
			delete gameObject;
		}
		deadGameObjects.clear();

		// Temp
		// Set the viewing transformation
		mat4 viewingTransformation = glm::lookAt(vec3(0.0f, 5.0f, 20.0f), vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewingTransformation));
		SharedProjectionAndViewing::setViewMatrix(viewingTransformation);

		SoundEngine::Update(deltaTime);

		// Save current time to determine when the scene should be rendered next
		lastRenderTime = currentTime;
	}

} // end updateGame()

void Game::renderScene()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto mesh : this->meshComps) {

		mesh->draw();

	}

	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(renderWindow);

} // end renderScene

//********************* Game Object Management *****************************************

void Game::addGameObject(GameObject* gameObject)
{
	// If updating gameObjects, need to add to pending
	if (this->updatingGameObjects) {
		pendingGameObjects.emplace_back(gameObject);
	}
	else {
		this->inGameObjects.emplace_back(gameObject);
	}

} // end addGameObject

void Game::removeGameObject(GameObject* gameObject)
{
	// Is it in pending gameObjects?
	auto iter = std::find(pendingGameObjects.begin(), pendingGameObjects.end(), gameObject);

	if (iter != pendingGameObjects.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, pendingGameObjects.end() - 1);
		pendingGameObjects.pop_back();
	}

	// Is it in this->inGameObjects?
	iter = std::find(this->inGameObjects.begin(), this->inGameObjects.end(), gameObject);

	if (iter != this->inGameObjects.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, this->inGameObjects.end() - 1);
		this->inGameObjects.pop_back();
	}

} // end removeGameObject


void Game::removeAndDeleteGameObject(GameObject* gameObject)
{
	this->removeGameObject(gameObject);

	delete gameObject;


} // end removeAndDeleteGameObject

GameObject* Game::findGameObjectByName(string name)
{
	for (auto i : this->inGameObjects) {
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

void Game::addMeshComp(MeshComponent* mesh)
{
	meshComps.emplace_back(mesh);
}

void Game::removeMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
	meshComps.erase(iter);

}

//********************* Shutdown Methods *****************************************

void Game::unloadData()
{
	// Delete gameObjects
	while (!this->inGameObjects.empty()) {

		delete this->inGameObjects.back();
	}

	while (!pendingGameObjects.empty()) {

		delete pendingGameObjects.back();
	}

} // end unloadData


void Game::shutdown()
{
	// Destroy the window
	glfwDestroyWindow(renderWindow);

	// Frees all remaining resources allocated by GLFW
	glfwTerminate();

	// Release all game resources that were loaded.
	unloadData();

	// Delete all shader programs that have been created
	deleteAllShaderPrograms();

} // end shutDown

//********************* Event Handlers *****************************************

void Game::window_close_callback(GLFWwindow* window) 
{
	// Stop the game loop
	this->isRunning = false;

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

		case GLFW_KEY_ESCAPE :

			// Stop the game loop
			this->isRunning = false;

			break;

		case GLFW_KEY_R: 

			roar->play(false);

			break;

		default:

			fprintf(stdout, "key pressed\n");
		}
	}

} // end key_callback


//********************* static function definitions *****************************************

/**
 * @fn	void error_callback(int error, const char* description)
 *
 * @brief	Callback, called when there is a GLFL error
 *
 * @param	error	   	The error.
 * @param	description	The description.
 */
void glfw_error_callback(int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);

} // end error_callback

/**
 * @fn	void displayOpenGlInfo(void)
 *
 * @brief	Function: Displays OpengGL and GLSL version information.
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

/**
 * @fn	void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
 *
 * @brief	Callback, called when the message
 *
 * @param	source   	Source for the.
 * @param	type	 	The type.
 * @param	id		 	The identifier.
 * @param	severity 	The severity. See https://www.khronos.org/registry/OpenGL/extensions/KHR/KHR_debug.txt for description.
 * @param	length   	The length.
 * @param	message  	The message.
 * @param	userParam	The user parameter.
 *
 * @returns	A GLAPIENTRY.
 */
void GLAPIENTRY openglMessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
								 const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);

} // end MessageCallback