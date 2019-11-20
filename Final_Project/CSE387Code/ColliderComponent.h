#pragma once

#include "Component.h"
#include "PhysicsEngine.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(class GameObject* owner);
	~ColliderComponent();
	//float GetRadius() const { return mRadius; }
	class btCollisionShape * GetCollisionShape() { return collisionShape; }
	void Initialize();
protected:

	class btCollisionShape * collisionShape;

	// Stores object space bounding sphere radius
	//float mRadius;
};

