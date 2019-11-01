#pragma once
#include "Component.h"
class SimpleMoveComponent : public Component
{
public:
	
	SimpleMoveComponent();

	virtual void update(float deltaTime) override;

	virtual void processInput() override;

protected:

	vec3 moveVector = ZERO_V3;

	float roarTimer = 0;

	float speed = 1.0;

	float rotationDegrees = 0.0f;
};

