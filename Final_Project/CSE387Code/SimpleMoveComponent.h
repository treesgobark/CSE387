#pragma once
#include "Component.h"
#include "CollisionComponent.h"


class SimpleMoveComponent : public CollisionComponent
{
public:
	
	SimpleMoveComponent();

	virtual void update(float deltaTime) override;

	virtual void processInput() override;

	virtual void CollisionEnter(const RigidBodyComponent* collisionData);

	virtual void CollisionExit(const RigidBodyComponent* collisionData){}

protected:

	vec3 moveVector = ZERO_V3;

	float roarTimer = 0;

	float speed = 1.0;

	float rotationDegrees = 0.0f;
};

