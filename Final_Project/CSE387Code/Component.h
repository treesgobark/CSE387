#pragma once

#include "GameObject.h"

enum COMPONENT_TYPE { COMPONENT = 0, MESH, COLLISION, CAMERA, LIGHT };

class Component
{
public:

	virtual COMPONENT_TYPE getComponentType() { return COMPONENT;  }

	/**
	 * @fn	Component::Component(int updateOrder = 100);
	 *
	 * @brief	Constructor (the lower the update order, the earlier the 
	 * 			component is initialized and updates)
	 *
	 * @param	updateOrder	(Optional) The update order.
	 */
	Component(int updateOrder = 100);

	/**
	 * @fn	virtual Component::~Component();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated data.
	 */
	virtual ~Component();

	/**
	 * @fn	virtual void Component::Update(float deltaTime);
	 *
	 * @brief	Update this component by delta time.
	 *
	 * @param	deltaTime	The time in seconds since the component was 
	 * 						last updated.
	 */
	virtual void update(float deltaTime);

	/**
	 * @fn	virtual void Component::ProcessInput()
	 *
	 * @brief	Process user input for this component
	 */
	virtual void processInput() {}

	/**
	 * @fn	virtual void Component::initialize();
	 *
	 * @brief	Initializes this component
	 */
	virtual void initialize();

	/**
	 * @fn	int Component::GetUpdateOrder() const
	 *
	 * @brief	Gets update order (the lower the update order, the earlier the 
	 * 			component is initialized and updates).
	 *
	 * @returns	The update order of this component.
	 */
	int GetUpdateOrder() const { return updateOrder; }


	/** @brief	Name of the component */
	string componentName;

	/**
	 * @fn	virtual bool MeshComponent::isMesh() override
	 *
	 * @brief	Enables GameObject AddComponent to quickly
	 * 			and efficiently determine if the component being
	 * 			added is a mesh and will need to be rendered on
	 * 			frame updates.
	 *
	 * @returns	True if mesh, false if not.
	 */
	//virtual bool isMesh() { return false; }


	/** @brief	friend declaration  
	 * Gives the GameObject class access to protected and private members
	 * of the Component class. Specifically, the game object addComponent
	 * method sets the owningGameObject member of this class.
	*/
	friend GameObject;

protected:

	/**
	 * @class	GameObject*
	 *
	 * @brief	A game object that this component is attached to
	 */
	class GameObject* owningGameObject;

	/** @brief	Initialization and update order of this component */
	int updateOrder;

}; // end Component
