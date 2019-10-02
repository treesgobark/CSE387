#pragma once
#include "MathLibsConstsFuncs.h"

class GameObject
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	GameObject(class Game* game);
	virtual ~GameObject();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);

	// Updates all the components attached to the GameObject (not overridable)
	void UpdateComponents(float deltaTime);

	// Any GameObject-specific update code (overridable)
	virtual void UpdateGameObject(float deltaTime);

	// ProcessInput function called from Game (not overridable)
	void ProcessInput();

	// Any GameObject-specific input code (overridable)
	virtual void GameObjectInput();

	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	std::vector<class Component*> getComponents() { return components; }

protected:

	// GameObject's state
	State gameObjectState;

	std::vector<class Component*> components;

	//class Game* game;
};
