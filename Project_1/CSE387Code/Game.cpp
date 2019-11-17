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

#include "SoundEngine.h"
#include "PhysicsEngine.h"
#include "CameraComponent.h"
#include "BoxMeshComponent.h"

//********************* Static Function declarations *****************************************

void glfw_error_callback(int error, const char* description);
void displayOpenGlInfo(void);
void GLAPIENTRY openglMessageCallback(	GLenum source, GLenum type, GLuint id, GLenum severity, 
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


Game::Game(std::string windowTitle)
	: windowTitle(windowTitle)
{

} // end Game Constructor

Game::~Game()
{
	SoundEngine::Stop();
	PhysicsEngine::Stop();
} // end Game Destructor

bool Game::initialize()
{
	bool windowInit = initializeRenderWindow();

	bool graphicsInit = initializeGraphics();

	bool soundInit = SoundEngine::Init();

	bool physicsInit = PhysicsEngine::Init();

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
	 // Set the clear color
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

	 SharedGeneralLighting::setAmbientColor(GL_LIGHT_ZERO, vec4(0.3f, 0.3f, 0.3f, 1.0f));
	 SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	 SharedGeneralLighting::setSpecularColor(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	 SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 0.0f));
	 SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, true);

	 GameObject* emptyGameObject = new GameObject(this);
	 sceneNode.addChild(emptyGameObject);
	 sceneNode.setPosition(vec3(0.0f, 0.0f, -10.0f), WORLD);

	 GameObject* dinoGameObject = new GameObject(this);
	 emptyGameObject->sceneNode.addChild(dinoGameObject);
	 ModelMeshComponent * dino1 = new ModelMeshComponent("Assets/Dinosaur/Trex.obj", shaderProgram);
	 dinoGameObject->addComponent(dino1);

	 CameraComponent* camera = new CameraComponent();
	 camera->setViewPort(0.0, 0.0, 1.0, 1.0); // full screen

	 //dinoGameObject = new GameObject(this);
	 //this->addChild(dinoGameObject);

	 //GameObject* emptyGameObject = new GameObject(this);
	 //dinoGameObject->addChild(emptyGameObject);
	 //emptyGameObject->sceneNode.setRotation(glm::rotate(PI, UNIT_Y_V3), WORLD);
	 //ModelMeshComponent* dino1 = new ModelMeshComponent("Assets/Dinosaur/Trex.obj", shaderProgram);
	 //rg = new RigidBodyComponent(dino1, DYNAMIC);
	 //emptyGameObject->addComponent(dino1);
	 //emptyGameObject->addComponent(rg);
	 ////emptyGameObject->addComponent(new SimpleMoveComponent());

	 //GameObject* boxGameObject = new GameObject(this);
	 //addChild(boxGameObject);
	 //boxGameObject->sceneNode.setPosition(vec3(5.5f, -3.0f, 0.0f));

	 //BoxMeshComponent* platform = new BoxMeshComponent(new Material(), 10.0f, 0.1f, 10.0f);
	 //boxGameObject->addComponent(platform);
	 //boxGameObject->addComponent(new RigidBodyComponent(platform, KINEMATIC_STATIONARY));

	 //GameObject* sphereGameObject = new GameObject(this);
	 //sceneNode.addChild(sphereGameObject);
	 //Material * material = new Material();
	 //material->setDiffuseMat(vec4(1.0f, 0.0, 0.0f, 1.0f));
	 //material->setSpecularMat(vec4(1.0f, 1.0f, 1.0f, 0.0f));
	 //material->setSpecularExponentMat(128.0f);
	 //material->setTextureMode(NO_TEXTURE);
	 //sphereGameObject->addComponent(new SphereMeshComponent(shaderProgram, material));

} // end loadData


void Game::initializeGameObjects()
{
	for (auto gameObject : sceneNode.children) {

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
	SceneNode::updatingGameObjects = true;

	// Update all the game objects in the game
	for (auto gameObject : sceneNode.children) {
	//for (auto gameObject : this->inGameObjects) {
		gameObject->processInput();
	}
	SceneNode::updatingGameObjects = false;

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

		PhysicsEngine::Update(deltaTime);

		// Update all gameObjects
		SceneNode::updatingGameObjects = true;

		for (auto gameObject : sceneNode.children) {

			gameObject->update(deltaTime);
		}

		SceneNode::updatingGameObjects = false;

		// Attach any pending game objects to their parent
		for (auto pending : SceneNode::pendingChildren) {

			// Initialize the pending GameObject
			pending->initialize();

			// Add the pending gameObject to the parent's child list
			pending->sceneNode.parent->children.emplace_back(pending);
		}
		SceneNode::pendingChildren.clear();

		// Delete dead game objects
		for (auto gameObject : SceneNode::deadGameObjects) {
			
			// Delete the dead game object from the parent's child list
			gameObject->sceneNode.parent->removeAndDeleteChild(gameObject);
		}

		// Clear the list for the next update cycle
		SceneNode::deadGameObjects.clear();

		// Set the viewing transformation
		mat4 viewingTransformation = glm::lookAt(vec3(0.0f, 5.0f, 20.0f), vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		SharedProjectionAndViewing::setViewMatrix(viewingTransformation);

		// Save current time to determine when the scene should be rendered next
		lastRenderTime = currentTime;
	}

} // end updateGame()

void Game::renderScene()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto camera : CameraComponent::activeCameras) {
		if (camera->isActive) {
			camera->setViewingTransformation();
			for (auto mesh : this->meshComps) {
				mesh->draw();
			}
		}
	}


	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(renderWindow);

} // end renderScene


GameObject* Game::findGameObjectByName(string name)
{
	// Traverse the scene graph to find a game object
	// TODO
	
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
	while (!this->sceneNode.children.empty()) {

		delete this->sceneNode.children.back();
	}

	while (!sceneNode.pendingChildren.empty()) {

		delete sceneNode.pendingChildren.back();
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

	// Set the projection transformation
	glm::mat4 projectionTransformation = glm::perspective(PI / 4.0f, ((float)width) / height, 0.5f, 1000.0f);

	SharedProjectionAndViewing::setProjectionMatrix(projectionTransformation);

} // end framebuffer_size_callback


void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {

		switch (key) {

		case GLFW_KEY_ESCAPE :

			// Stop the game loop
			this->isRunning = false;

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