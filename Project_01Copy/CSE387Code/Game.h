#pragma once
#include <string>
#include "MathLibsConstsFuncs.h"
#include <GLFW/glfw3.h>


class Game
{

public:
	Game(std::string windowTitle = "CSE387");

	~Game();

	bool initialize();

	bool initializeRenderWindow();

	bool initializeGraphics();

	//used to bind call back functions
	void bindCallBacks();

	void addGameObject(class GameObject* gameObject);

	void removeGameObject(class GameObject* gameObject);
	
	void gameLoop();

	void shutdown();

	virtual void loadData();
	virtual void unloadData();

	const std::string& GetWindowTitle() { return windowTitle; }

	GameObject* findGameObjectByName(string name);

	void InitializeGameObjects();

	void window_close_callback(GLFWwindow* window);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void AddMeshComp(class Mesh* mesh);
	void RemoveMeshComp(class Mesh* mesh);

protected:

	virtual void processInput();
	virtual void updateGame();
	void renderScene();

	//virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	// All the actors in the game
	std::vector<class GameObject*> inGameObjects;

	// Any pending actors
	std::vector<class GameObject*> pendingGameObjects;

	//Uint32 mTicksCount;
	bool isRunning;

	// Track if we're updating gameObjects right now
	bool updatingGameObjects;

	// All mesh components drawn
	std::vector<class Mesh*> meshComps;

	// Title for the window
	std::string windowTitle;

	GLFWwindow* renderWindow; // The rendering window

};
