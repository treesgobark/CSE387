#pragma once
#include <string>

#include "MathLibsConstsFuncs.h"
#include <GLFW/glfw3.h>

#include "SceneNode.h"

static const int initialScreenWidth = 1024;
static const int initialScreenHeight = 768;

static const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
static const GLdouble FRAME_INTERVAL = 1.0 / FRAMES_PER_SECOND; // Interval in milliseconds between frames

class Game
{

public:

	/**
	 * @fn	Game::Game(std::string windowTitle = "CSE387");
	 *
	 * @brief	Constructor
	 *
	 * @param	windowTitle	(Optional) title for the rendering window.
	 */
	Game(std::string windowTitle = "CSE387");

	/**
	 * @fn	Game::~Game();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated memory 
	 * 			and release all resources.
	 */
	~Game();

	/**
	 * @fn	bool Game::initialize();
	 *
	 * @brief	Initializes graphics as well as sound and physics
	 * 			engines. Loads the initial scene and initializes all 
	 * 			game objects.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool initialize();

	/**
	 * @fn	void Game::gameLoop();
	 *
	 * @brief	Game loop. Repeated processes user input, updates
	 * 			all game objects, and renders the scene until isRunning
	 * 			is false and the game ends.
	 */
	void gameLoop();

	/**
	 * @fn	void Game::addMeshComp(class MeshComponent* mesh);
	 *
	 * @brief	Adds a mesh component to the list of components
	 * 			that must be rendered visually.
	 *
	 * @param [in]	mesh	If non-null, the mesh.
	 */
	void addMeshComp(class MeshComponent* mesh);

	/**
	 * @fn	void Game::removeMeshComp(class MeshComponent* mesh);
	 *
	 * @brief	Removes the mesh component from the list of components
	 * 			that must be rendered visually.
	 *
	 * @param [in]	mesh	If non-null, the mesh.
	 */
	void removeMeshComp(class MeshComponent* mesh);

	/**
	 * @fn	void Game::shutdown();
	 *
	 * @brief	Deletes all game objects in the game.
	 */
	void shutdown();

	/**
	 * @fn	const std::string& Game::GetWindowTitle()
	 *
	 * @brief	Returns the window title.
	 *
	 * @returns	The window title.
	 */
	const std::string& GetWindowTitle() { return windowTitle; }

	/**
	 * @fn	GameObject* Game::findGameObjectByName(string name);
	 *
	 * @brief	Searches for the first game object by name.
	 *
	 * @param	name	The name of the game object being searched
	 * 					for.
	 *
	 * @returns	Null if it fails, else the found game object by name.
	 */
	GameObject* findGameObjectByName(string name);

	/**
	 * @fn	void Game::window_close_callback(GLFWwindow* window);
	 *
	 * @brief	Callback, called when the window is closed. Sets
	 * 			isRunning to false to stop the game loop.
	 *
	 * @param [in]	window	If non-null, the window being closed.
	 */
	void window_close_callback(GLFWwindow* window);

	/**
	 * @fn	void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height);
	 *
	 * @brief	Callback, called when the framebuffer is resized; usually the result
	 * 			of the rendering window being resized.
	 *
	 * @param [in,out]	window	If non-null, the window containing the framebuffer
	 * @param 		  	width 	The width in pixels of the framebuffer.
	 * @param 		  	height	The height in pixels of the framebuffer.
	 */
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	/**
	 * @fn	void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	 *
	 * @brief	Callback, called when the key
	 *
	 * @param [in,out]	window  	If non-null, the window.
	 * @param 		  	key			The key that was pressed, released, or held down.
	 * @param 		  	scancode	The scancode.
	 * @param 		  	action  	The action.
	 * @param 		  	mods		The mods.
	 */
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Friend declaration give the GameObject class access to protected and 
	// private members of the Game class.
	friend GameObject;

protected:

	class SceneNode sceneNode;

	/**
	 * @fn	bool Game::initializeRenderWindow();
	 *
	 * @brief	Initializes the rendering window 
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool initializeRenderWindow();

	/**
	 * @fn	void Game::bindCallBacks();
	 *
	 * @brief	used to bind all call back functions
	 */
	void bindCallBacks();

	/**
	 * @fn	bool Game::initializeGraphics();
	 *
	 * @brief	Initializes the rendering API
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool initializeGraphics();

	/**
	 * @fn	virtual void Game::loadData();
	 *
	 * @brief	Creates game objects and attaches components. Created
	 * 			game objects are added to the game.
	 */
	virtual void loadData();

	/**
	 * @fn	void Game::InitializeGameObjects();
	 *
	 * @brief	Initializes all game objects that are presently in the 
	 * 			game. All game objects need to be initialized. Those that
	 * 			are added after the game has started must be initialized 
	 * 			individually.
	 */
	void initializeGameObjects();

	/**
	 * @fn	virtual void Game::processInput();
	 *
	 * @brief	Process the input and passes it to the game objects and
	 * 			componenets that are in the game.
	 */
	virtual void processInput();

	/**
	 * @fn	virtual void Game::updateGame();
	 *
	 * @brief	Updates all game objects and the attached components.
	 */
	virtual void updateGame();

	/**
	 * @fn	void Game::renderScene();
	 *
	 * @brief	Renders the game scene.
	 */
	void renderScene();

	/**
	 * @fn	virtual void Game::unloadData();
	 *
	 * @brief	Deletes all games objects and any attached
	 * 			components.
	 */
	virtual void unloadData();

	/** @brief	True if the game is running, false if not */
	bool isRunning = true;

	/** @brief	All mesh components that need to be rendered */
	std::vector<class MeshComponent*> meshComps;

	/** @brief	Title for the window */
	std::string windowTitle;

	/** @brief	The rendering window */
	GLFWwindow* renderWindow = NULL;

}; // end game class
