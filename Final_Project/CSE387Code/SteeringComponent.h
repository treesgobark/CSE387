#pragma once
#include "WaypointComponent.h"

class SteeringComponent : public WaypointComponent
{
public:
	SteeringComponent(std::vector< glm::vec3> waypoints, vec3 velocity = vec3(10, 0, 0));

	virtual void update(float deltaTime) override;

}; // end SteeringComponent class


