#pragma once
#include "Component.h"
class KinematicsComponent :
	public Component
{
	KinematicsComponent(int updateOrder = 100);

	virtual void update(float deltaTime);

	vec3 position = vec3(0.0f, 0.0f, 0.0f);
	vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
	vec3 acceleration = vec3(1.0f, 0.0f, 0.0f);
};

