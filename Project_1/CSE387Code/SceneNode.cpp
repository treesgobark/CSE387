#include "SceneNode.h"
#include "GameObject.h"


// ***** Definitions of static data members that are shared by all SceneNode objects ****
bool SceneNode::updatingGameObjects = false;

std::vector<class GameObject*> SceneNode::deadGameObjects;

std::vector<class GameObject*> SceneNode::pendingChildren;

// **************************************************************************************


glm::vec3 SceneNode::getPosition(Frame frame)
{
	glm::vec3 position;

	return position;

} // end getPosition

glm::mat4 SceneNode::getRotation(Frame frame)
{
	// TODO

	return glm::mat4(1.0f);

} // getRotation

glm::mat4 SceneNode::getScale(Frame frame)
{
	// TODO

	return glm::mat4(1.0f);

} // end getScale


glm::mat4 SceneNode::getTransformation(Frame frame)
{
	// TODO

	return glm::mat4(1.0f);

} // end getTransform


void SceneNode::setPosition(glm::vec3 position, Frame frame)
{
	// TODO


} // end setPosition


void SceneNode::setRotation(glm::mat4 rotation, Frame frame)
{
	// TODO

} // end setRotation


void SceneNode::setScale(const vec3& scale, Frame frame)
{
	// TODO

} // end setScale


void SceneNode::setTransform(glm::mat4 transformation, Frame frame)
{
	// TODO

} // end setTransform


glm::vec3 SceneNode::getFowardDirection(Frame frame)
{
	// TODO

	return glm::vec3(0.0f, 0.0f, 0.0f);

} // getForwardDirection


glm::vec3 SceneNode::getUpDirection(Frame frame)
{
	// TODO

	return glm::vec3(0.0f, 0.0f, 0.0f);

} // getUpDirection


glm::vec3 SceneNode::getRightDirection(Frame frame)
{
	// TODO

	return glm::vec3(0.0f, 0.0f, 0.0f);

} // getRightDirection


glm::mat4 SceneNode::getParentWorldTransform()
{
	// TODO

	return glm::mat4(1.0f);

} // end getParentWorldTransform


glm::mat4 SceneNode::getModelingTransformation()
{
	return getTransformation(WORLD) * scale;

} // end getModelingTransformation


void SceneNode::reparent(GameObject* child)
{
	// TODO

} // end reparent


void SceneNode::addChild(GameObject* gameObject)
{
	// Set the parent for the sceneNode of the gameObject
	gameObject->sceneNode.parent = this;

	// If updating gameObjects, need to add to pending
	// for later addition the child list of the parent
	// and initialization.
	if ( this->updatingGameObjects == true ) {

		pendingChildren.emplace_back(gameObject);
	}
	else {

		// Initialize the game object and add to the
		// child list of the parent
		gameObject->initialize();
		this->children.emplace_back(gameObject);
	}

} // end addGameObject


void SceneNode::removeChild(GameObject* gameObject)
{
	// Is it in pending gameObjects?
	auto iter = std::find(pendingChildren.begin(), pendingChildren.end(), gameObject);

	if (iter != pendingChildren.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, pendingChildren.end() - 1);
		pendingChildren.pop_back();
	}

	// Is it in this->inGameObjects?
	iter = std::find(this->children.begin(), this->children.end(), gameObject);

	if (iter != this->children.end()) {
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, this->children.end() - 1);
		this->children.pop_back();
	}

} // end removeGameObject


void SceneNode::removeAndDeleteChild(GameObject* gameObject)
{
	this->removeChild(gameObject);

	delete gameObject;

} // end removeAndDeleteGameObject

void SceneNode::rotateTo(const glm::vec3& direction, Frame frame)
{
	// Normalize the new direction
	vec3 newDirection = glm::normalize(direction);

	if (frame == WORLD) {

		// Transform the direction to local coordinates
		newDirection = (glm::inverse(this->getParentWorldTransform()) * glm::vec4(newDirection, 0.0f)).xyz;
	}

	// Find the perpendicular vector to rotate about
	glm::vec3 axis = glm::cross(FORWARD, newDirection);

	// Check if direction is equal to FORWARD
	if (glm::length(axis) == 0.0f) {

		setRotation(mat3(1.0f), LOCAL);
	}
	else {

		// Find the angle to rotate between the current direction and the new direction
		float angle = glm::acos(glm::dot(FORWARD, newDirection));

		// Set the local orientation
		mat3 localRot = mat3(glm::rotate(angle, axis));

		// Set the local transformation
		setRotation(localRot, LOCAL);
	}
} // end rotateTo
