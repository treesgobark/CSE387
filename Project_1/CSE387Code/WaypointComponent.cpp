#include "WaypointComponent.h"


WaypointComponent::WaypointComponent(std::vector< glm::vec3> waypoints, vec3 velocity)
	: velocity(velocity), speed(glm::length(velocity)), waypoints(waypoints), 
	targetWaypointIndex(static_cast<int>(waypoints.size()) - 1)
{
	targetWaypointIndex = getNexWaypointIndex();

}

void WaypointComponent::update(float deltaTime)
{
	float radius = 3.0;

	// Check if next waypoint has been reached
	if (distanceToTargetWaypoint() < (speed * deltaTime * radius)) {
		//cout << waypoints[targetWaypointIndex] << endl;
		targetWaypointIndex = getNexWaypointIndex();
		//cout << "change " << owningGameObject->sceneNode.getPosition(WORLD) << endl;
	}


	vec3 current = owningGameObject->sceneNode.getFowardDirection(WORLD);

	vec3 desiredDirection = getDirectionToNextWaypoint();

	vec3 position = owningGameObject->sceneNode.getPosition(WORLD);

	position = position + desiredDirection * speed * deltaTime;

	owningGameObject->sceneNode.setPosition(position, WORLD);

} // end update


int WaypointComponent::getNexWaypointIndex()
{
	return (targetWaypointIndex + 1) % waypoints.size();

} // end getNexWaypointIndex

vec3 WaypointComponent::getDirectionToNextWaypoint()
{
	return glm::normalize((waypoints[targetWaypointIndex] - owningGameObject->sceneNode.getPosition(WORLD)));

} // end getDirectionToNextWaypoint

GLfloat WaypointComponent::distanceToTargetWaypoint()
{
	return glm::distance(waypoints[targetWaypointIndex], owningGameObject->sceneNode.getPosition(WORLD));

} // end distanceToTargetWaypoint