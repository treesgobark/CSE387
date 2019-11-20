#include "SteeringComponent.h"




SteeringComponent::SteeringComponent(std::vector< glm::vec3> waypoints, vec3 velocity)
	: WaypointComponent(waypoints, velocity)
{

}

void SteeringComponent::update(float deltaTime)
{
	// Check if next waypoint has been reached
	if (distanceToTargetWaypoint() < (speed * deltaTime * 30)) {
		targetWaypointIndex = getNexWaypointIndex();
	}

	// Get the current facing direction
	vec3 facingDirection = owningGameObject->sceneNode.getFowardDirection(WORLD);

	// Calculate current yaw angle
	float currentYaw = atan2(-facingDirection.x, -facingDirection.z);

	// Calculate current pitch angle
	float currentPitch = atan2(facingDirection.y, glm::sqrt(glm::pow(facingDirection.x, 2) + pow(facingDirection.z, 2)));

	// Figure out direction to next waypoint
	vec3 desiredDirection = getDirectionToNextWaypoint();

	// Get heading angle to the next waypoint
	float desiredYaw = atan2(-desiredDirection.x, -desiredDirection.z);

	// Account for the "branch cut" in yaw angles
	if (fabs(currentYaw - desiredYaw) > PI) {

		if (currentYaw < desiredYaw) {

			currentYaw += 2 * PI;
		}
		else {
			desiredYaw += 2 * PI;
		}
	}

	// Determine the desired pitch
	float desiredPitch = atan2(desiredDirection.y, glm::sqrt(glm::pow(desiredDirection.x, 2) + pow(desiredDirection.z, 2)));

	static float currentRoll = 0;
	float desiredRoll = 0;

	// Determine the turn direction and set the roll angle accordingly
	if (desiredYaw < currentYaw) {
		// Left turn
		desiredRoll = fabs(desiredYaw - currentYaw);
	}
	else {
		// Right turn
		desiredRoll = -fabs(desiredYaw - currentYaw);
	}

	// Interpolate towards desired roll, pitch, and yaw angles
	currentRoll = glm::lerp(currentRoll, desiredRoll, deltaTime);
	float yaw = glm::lerp(currentYaw, desiredYaw, deltaTime/2);
	float pitch = glm::lerp(currentPitch, desiredPitch, deltaTime/2);

	// Apply Euler angles
	mat3 localRot = glm::rotate(yaw, UP) * glm::rotate(pitch, RIGHT) * glm::rotate(currentRoll, FORWARD);

	// Set the orientation of the owning game object
	owningGameObject->sceneNode.setRotation(localRot,WORLD);

	// Get the new facing direction after the orientation has been applied
	vec3 newFacingDirection = owningGameObject->sceneNode.getFowardDirection(WORLD);

	// Get the current position
	vec3 position = owningGameObject->sceneNode.getPosition(WORLD);

	// Determine the new direction based on the speed
	position = position + newFacingDirection * speed * deltaTime;

	// Update the position of the owning game object
	this->owningGameObject->sceneNode.setPosition(position, WORLD);

} // end update


