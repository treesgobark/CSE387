#pragma once
#include <cstdint>
#include "GameObject.h"
#include "SharedProjectionAndViewing.h"

class Component
{
public:
	// Constructor
	// (the lower the update order, the earlier the component updates)
	Component(class GameObject* owner, int updateOrder = 100);

	// Destructor
	virtual ~Component();

	// Update this component by delta time
	virtual void Update(float deltaTime);

	// Process input for this component
	virtual void ProcessInput() {}

	virtual void Initialize();

	int GetUpdateOrder() const { return updateOrder; }

	string componentName;

protected:

	// Owning GameObject
	class GameObject* owner;

	// Update order of component
	int updateOrder;

};
