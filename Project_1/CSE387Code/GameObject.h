#pragma once
#include "MathLibsConstsFuncs.h"
#include "Game.h"

#include "SceneNode.h"


/**
 * @enum	State
 *
 * @brief	Values that represent states
 * 			ACTIVE - Game object is updated and "rendered"
 * 			PAUSED - Game object is not updated or "rendered"
 * 			DEAD - Game object will be removed from the game.
 */
enum STATE { ACTIVE, PAUSED, DEAD };

class GameObject
{
public:

	/**
	 * @fn	Game* getOwningGame()
	 *
	 * @brief	Gets a reference to the game that this gameObject
	 * 			is a part of.
	 *
	 * @returns	Null if it fails, else the owning game.
	 */
	class Game* getOwningGame() { return this->owningGame; }


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
	STATE getState() const { return gameObjectState; }

	/**
	 * @fn	void setState(State state);
	 *
	 * @brief	Sets a state of the game object. Only ACTIVE game
	 * 			objects are updated and rendered. When the state of 
	 * 			an object is set to DEAD, it will be deleted on the
	 * 			next update cycle.
	 *
	 * @param	state	The state to which the object is being set
	 */
	void setState(STATE state);

	void addChild(GameObject* child);

	void removeChild(GameObject* child);

	void removeAndDeleteChild(GameObject* child);

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

	/**
	 * @class	SceneNode
	 *
	 * @brief	A scene graph node.
	 */
	class SceneNode sceneNode;

protected:

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
	STATE gameObjectState;

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
