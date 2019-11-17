#include "SceneNode.h"
#include "GameObject.h"
#include "MeshComponent.h"


// ***** Definitions of static data members that are shared by all SceneNode objects ****
bool SceneNode::updatingGameObjects = false;

std::vector<class GameObject*> SceneNode::deadGameObjects;

std::vector<class GameObject*> SceneNode::pendingChildren;

// **************************************************************************************


glm::vec3 SceneNode::getPosition(Frame frame)
{
	// apparently referencing parent in any way throws an access violation for some reason
	//if (frame == WORLD) {
	//	if (!parent) return getPosition();
	//	return parent->getPosition(WORLD) + getPosition();
	//}

	//float scalingFactor = getScale()[0][0];

	//mat4 rotationMat = getRotation();

	return vec3(0.0f);

} // end getPosition

glm::mat4 SceneNode::getRotation(Frame frame)
{
	// apparently referencing parent in any way throws an access violation for some reason
	//if (frame == WORLD) {
	//	if (!parent) return getRotation();
	//}
	return (1.0f / scale[0][0]) * localTransform;
} // getRotation

glm::mat4 SceneNode::getScale(Frame frame)
{
	// apparently referencing parent in any way throws an access violation for some reason
	//if (frame == WORLD) {
	//	if (!parent) return scale;
	//	return parent->getScale(WORLD) * scale;
	//}
	return glm::scale(vec3(sqrt(localTransform[0][0] * localTransform[0][0] + localTransform[1][1] * localTransform[1][1] + localTransform[2][2] * localTransform[2][2])));
} // end getScale


glm::mat4 SceneNode::getTransformation(Frame frame)
{
	// apparently referencing parent in any way throws an access violation for some reason
	if (frame == WORLD) {
		//if (getParent() == NULL) return getTransformation();
	}

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
	// apparently referencing parent in any way throws an access violation for some reason
	if (parent) return parent->getTransformation(WORLD);

	return getTransformation();

} // end getParentWorldTransform


glm::mat4 SceneNode::getModelingTransformation()
{
	return getTransformation(WORLD) * scale;

} // end getModelingTransformation


void SceneNode::reparent(GameObject* child)
{
	// Remove the child for its old parent's list of children
	child->sceneNode.getParent()->removeChild(child);
	addChild(child);

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

		// add to the child list of the parent
		this->children.emplace_back(gameObject);
	}

	// Search for any mesh components to make sure the have been added to the scene
	// graph. This is necessary because of the possibility of reparenting.
	for (auto comp : gameObject->getComponents()) {

		// Check if this Component is a Mesh 
		if (comp->isMesh() == true) {

			gameObject->getOwningGame()->addMeshComp(dynamic_cast<MeshComponent*>(comp));

		}
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

	// Search for any mesh components to keep them from rendering
	// and remove them to keep them from rendering.
	for (auto comp : gameObject->getComponents()) {

		// Check if this Component is a Mesh 
		if (comp->isMesh() == true) {
			gameObject->getOwningGame()->removeMeshComp(dynamic_cast<MeshComponent*>(comp));
		}
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

void SceneNode::updateSceneGraph(float deltaTime)
{
	// Update all gameObjects
	SceneNode::updatingGameObjects = true;

	// Update all the game objects in the game
	for (auto gameObject : this->children) {

		gameObject->processInput();
	}

	for (auto gameObject : this->children) {

		gameObject->update(deltaTime);
	}

	SceneNode::updatingGameObjects = false;

	// Attach any pending game objects to their parent
	for (auto pending : SceneNode::pendingChildren) {

		// Add the pending gameObject to the parent's child list
		pending->sceneNode.parent->children.emplace_back(pending);
	}
	SceneNode::pendingChildren.clear();

	// Delete dead game objects
	for (auto gameObject : SceneNode::deadGameObjects) {

		// Delete the dead game object from the parent's child list
		gameObject->sceneNode.parent->removeAndDeleteChild(gameObject);

		delete gameObject;
	}

	// Clear the list for the next update cycle
	SceneNode::deadGameObjects.clear();

} // end updateSceneGraph

