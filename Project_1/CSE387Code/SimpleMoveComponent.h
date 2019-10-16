#pragma once
#include "Component.h"
class SimpleMoveComponent : public Component
{
public:
	
	SimpleMoveComponent();

	virtual void update(float deltaTime) override;

	virtual void processInput() override;

protected:

	// keep at unit length
	vec3 moveDirection = ZERO_V3;

	float speed = 3.0;

	float roarTimer = 0;

};

