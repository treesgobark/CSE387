#pragma once

#include "Component.h"

/**
 * 	 std::vector<vec3> waypoints = { vec3(0.0f, 0.0f, 0.0f),vec3(40.0f, 0.0f, -40.0f), vec3(-40.0f, 0.0f, -40.0f) };
 *	 WaypointComponent * wpc = new WaypointComponent(waypoints);
 *	 dinoGameObject->addComponent(wpc);
 * 
*/

class WaypointComponent : 	public Component
{
public:

	/**
	 * @fn	WaypointComponent::WaypointComponent(std::vector< glm::vec3> waypoints, vec3 velocity = vec3(10, 0, 0));
	 *
	 * @brief	Constructor
	 *
	 * @param	waypoints	The waypoints.
	 * @param	velocity 	(Optional) The velocity.
	 */
	WaypointComponent(std::vector< glm::vec3> waypoints, vec3 velocity = vec3(10, 0, 0));

	virtual void update(float deltaTime) override;

protected:

	int getNexWaypointIndex();
	vec3 getDirectionToNextWaypoint();
	GLfloat distanceToTargetWaypoint();

	std::vector< glm::vec3> waypoints;

	vec3 velocity;
	float speed;

	int targetWaypointIndex;


};

