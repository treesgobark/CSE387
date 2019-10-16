#pragma once
#include "MathLibsConstsFuncs.h"
#include "Game.h"

class GameObject
{
public:

	/**
	 * @enum	State
	 *
	 * @brief	Values that represent states
	 * 			ACTIVE - Game object is updated and "rendered"  
	 * 			PAUSED - Game object is not updated or "rendered"  
	 * 			DEAD - Game object will be removed from the game.
	 */
	enum State
	{
		ACTIVE,
		PAUSED,
		DEAD
	};

	/**
	 * @fn	GameObject::GameObject(class Game * game);
	 *
	 * @brief	Constructor
	 *
	 * @param [in}	game that this game object will be part of
	 */
	GameObject(class Game * game);

	/**
	 * @fn	virtual ~GameObject();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated memory.
	 */
	virtual ~GameObject();

	/**
	 * @fn	virtual void initialize();
	 *
	 * @brief	Initializes this game object and all attached components
	 * 			and any children of the game object.
	 * 	
	 */
	virtual void initialize();

	/**
	 * @fn	void Update(float deltaTime);
	 *
	 * @brief	Update function called from Game (not overridable). Updates
	 * 			this for game object, all attached components, and any 
	 * 			child game objects. 
	 *
	 * @param	deltaTime	The time since the last update in seconds.
	 */
	void update(float deltaTime);

	/**
	 * @fn	void ProcessInput();
	 *
	 * @brief	ProcessInput method called from Game (not overridable).
	 * 			Processes input for this game object, all attached 
	 * 			components, and any child game objects. 
	 */
	void processInput();

	/**
	 * @fn	void AddComponent(class Component* component);
	 *
	 * @brief	Attaches a component to this game object
	 *
	 * @param [in,out]	component	If non-null, the component to
	 * 					attached.
	 */
	void addComponent(class Component* component);

	/**
	 * @fn	void removeComponent(class Component* component);
	 *
	 * @brief	Detaches the component described by component from this
	 * 			game object
	 *
	 * @param [in,out]	component	If non-null, the component to be 
	 * 					detached.
	 */
	void removeComponent(class Component* component);

	/**
	 * @fn	std::vector<class Component*> getComponents()
	 *
	 * @brief	Gets all components attached to this game object.
	 *
	 * @returns	Null if it fails, else the components attached to the
	 * 			game object.
	 */
	std::vector<class Component*> getComponents() { return components; }

	/**
	 * @fn	State getState() const
	 *
	 * @brief	Gets the state of the game object.
	 *
	 * @returns	The state of the game object.
	 */
	State getState() const { return gameObjectState; }

	/**
	 * @fn	string getName()
	 *
	 * @brief	Gets the name of the game object.
	 *
	 * @returns	The name.
	 */
	string getName() { return objectName; }

	/**
	 * @fn	void setName(string name)
	 *
	 * @brief	Sets a name of the game object
	 *
	 * @param	name	The name that the game object will be
	 * 					given.
	 */
	void setName(string name) { objectName = name; }

	// Local transform that expresses the position, orientation,
	// and scale of this game object relative to its parent.
	mat4 localTransform = mat4(1.0f);

protected:

	/**
	 * @fn	void UpdateComponents(float deltaTime);
	 *
	 * @brief	Updates all the components attached to the GameObject (not overridable)
	 *
	 * @param	deltaTime	 time since the last update in seconds.
	 */
	void updateComponents(float deltaTime);

	/**
	 * @fn	virtual void UpdateGameObject(float deltaTime);
	 *
	 * @brief	Any GameObject-specific update code (overridable)
	 *
	 * @param	deltaTime	time since the last update in seconds.
	 */
	virtual void updateGameObject(float deltaTime);

	/**
	 * @fn	virtual void GameObjectInput();
	 *
	 * @brief	Any GameObject-specific input code (overridable). Called 
	 * 			by processInput
	 */
	virtual void gameObjectInput();

	/** @brief	Current state of the game object */
	State gameObjectState;

	/** @brief	The components that are attached to this game object. */
	std::vector<class Component*> components;

	/**
	 * @class	Game*
	 *
	 * @brief	The game that this game object is part of.
	 */
	class Game* owningGame;

	/** @brief	Name of the game object */
	string objectName;

	/** @brief	The label of this game object*/
	string objectLabel;

}; // end GameObject class
