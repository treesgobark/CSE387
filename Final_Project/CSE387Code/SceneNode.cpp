#include "SceneNode.h"
#include "GameObject.h"
#include "MeshComponent.h"


// ***** Definitions of static data members that are shared by all SceneNode objects ****
bool SceneGraphNode::updatingGameObjects = false;

std::vector<class GameObject*> SceneGraphNode::deadGameObjects;

std::vector<class GameObject*> SceneGraphNode::pendingChildren;

// **************************************************************************************


glm::vec3 SceneGraphNode::getPosition(Frame frame)
{
	glm::vec3 position;

	if (frame == Frame::LOCAL) {

		// Get the position in local coordinates
		position = getPositionVec3FromTransform(localTransform);
	}
	else {
		position = getPositionVec3FromTransform(getTransformation(WORLD));
	}
	return position;

} // end getPosition

glm::mat4 SceneGraphNode::getRotation(Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Get the rotation in local coordinates
		return getRotationMatrixFromTransform(localTransform);
	}
	else {

		return getRotationMatrixFromTransform(getTransformation(WORLD));
	}

} // getRotation

glm::mat4 SceneGraphNode::getScale(Frame frame)
{
	if( frame == Frame::LOCAL ) {
		// Get the scale in local coordinates
		return this->scale;
	}
	else {
		vec3 worldScaleValues = getScaleFromTransform(getTransformation(WORLD));
		return glm::scale(worldScaleValues);
	}
	 
} // end getScale


glm::mat4 SceneGraphNode::getTransformation(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return localTransform;
	}
	else {
		return getParentWorldTransform() * localTransform;
	}

} // end getTransform


void SceneGraphNode::setPosition(glm::vec3 position, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Set the position in local coordinates
		setPositionVec3ForTransform(localTransform, position);
	}
	else {

		mat4 worldT = getTransformation(WORLD);
		mat4 invParentT = glm::inverse(getParentWorldTransform());
		setPositionVec3ForTransform(worldT, position);
		localTransform = invParentT * worldT;
	}

} // end setPosition


void SceneGraphNode::setRotation(glm::mat4 rotation, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Set the rotation in local coordinates
		setRotationMat3ForTransform(localTransform, rotation);
	}
	else {
		// Set the rotation in world coordinates
		if (this->parent == nullptr) {
			setRotationMat3ForTransform(localTransform, rotation);
		}
		else {
			glm::mat4 parentWorldRotation = this->parent->getRotation(Frame::WORLD);
			glm::mat4 newRoatation = glm::inverse(parentWorldRotation) * rotation;
			setRotationMat3ForTransform(localTransform, newRoatation);
		}
	}

} // end setRotation


void SceneGraphNode::setScale(const vec3& scale, Frame frame)
{
	this->scale = glm::scale(scale);

} // end setScale


void SceneGraphNode::setTransform(glm::mat4 transformation, Frame frame)
{
	if (frame == Frame::LOCAL) {

		// Set the rotation in local coordinates
		this->localTransform = transformation;
	}
	else {
		// Set the rotation in world coordinates
		if (this->parent == nullptr) {
			this->localTransform = transformation;
		}
		else {
			glm::mat4 parentWorldTransformation = getParentWorldTransform();
			this->localTransform = glm::inverse(parentWorldTransformation) * transformation;
		}
	}

} // end setTransform


glm::vec3 SceneGraphNode::getFowardDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return FORWARD;
	}
	else {
		return glm::mat3(getRotation(frame)) * FORWARD;
	}

} // getForwardDirection


glm::vec3 SceneGraphNode::getUpDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return UP;
	}
	else {
		return glm::mat3(getRotation(frame)) * UP;
	}

} // getUpDirection


glm::vec3 SceneGraphNode::getRightDirection(Frame frame)
{
	if (frame == Frame::LOCAL) {
		return RIGHT;
	}
	else {
		return glm::mat3(getRotation(frame)) * RIGHT;
	}

} // getRightDirection


glm::mat4 SceneGraphNode::getParentWorldTransform()
{
	if (this->parent == nullptr) {
		return mat4(1.0f);
	}
	else {
		if (parent->applyScaleToChildren == true) {
			return parent->getTransformation(WORLD) * parent->scale;
		}
		else {
			return parent->getTransformation(WORLD);
		}
	}

} // end getParentWorldTransform


glm::mat4 SceneGraphNode::getModelingTransformation()
{
	return getTransformation(WORLD) * scale;

} // end getModelingTransformation

void SceneGraphNode::rotateTo(const glm::vec3& direction, Frame frame)
{
	// Normalize the new direction
	vec3 newDirection = glm::normalize(direction);

	if (frame == WORLD) {

		// Transform the direction to local coordinates
		newDirection = (glm::inverse(getParentWorldTransform()) * glm::vec4(newDirection, 0.0f)).xyz;
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

void SceneGraphNode::eulerRotate(const float& yawAngle, const float& pitchAngle, const float& rollAngle, Frame frame)
{
	mat3 localRot = mat3(glm::rotate(yawAngle, UP) *
		glm::rotate(pitchAngle, RIGHT) *
		glm::rotate(rollAngle, FORWARD));

	setRotation(localRot, frame);

} // end setRotation


void SceneGraphNode::reparent(GameObject* child)
{
	// Get the previous world transform of the child
	glm::mat4 oldWorldTransformation = child->sceneNode.getTransformation(WORLD);

	// Remove the child for its old parent's list of children
	child->sceneNode.getParent()->removeGameObjectFromSceneGraph(child);

	// Add the child to the new parent's list of children
	this->addGameObjectToSceneGraph(child);

	// Get inverse of the old world of the new parent world transformation and old child world.
	glm::mat4 newTransform = glm::inverse(this->getTransformation(WORLD)) * oldWorldTransformation;

	// Overwrite the position scale and rotation 
	child->sceneNode.localTransform = newTransform;

} // end reparent


void SceneGraphNode::addGameObjectToSceneGraph(GameObject* gameObject)
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
		this->children.emplace_back(gameObject);
	}

	// Search for any mesh components to make sure the have been added to the scene
	// graph. This is necessary becasue of the possibility of reparenting.
	for (auto comp : gameObject->getComponents()) {

		// Check if this Component is a Mesh 
		if (comp->getComponentType() == MESH) {

			gameObject->getOwningGame()->addMeshComp(dynamic_cast<MeshComponent*>(comp));
		}
	}

} // end addGameObject


void SceneGraphNode::removeGameObjectFromSceneGraph(GameObject* gameObject)
{
	// Is it in pending gameObjects?
	auto iter = std::find(SceneGraphNode::pendingChildren.begin(), 
					      SceneGraphNode::pendingChildren.end(), gameObject);

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
		if (comp->getComponentType() == MESH) {

			gameObject->getOwningGame()->removeMeshComp(dynamic_cast<MeshComponent*>(comp));
		}
	}

} // end removeGameObject


void SceneGraphNode::updateSceneGraph(float deltaTime)
{
	// Update all gameObjects
	SceneGraphNode::updatingGameObjects = true;

	// Update all the game objects in the game
	for (auto gameObject : this->children) {

		gameObject->processInput();
	}

	for (auto gameObject : this->children) {

		gameObject->update(deltaTime);
	}

	SceneGraphNode::updatingGameObjects = false;

	// Attach any pending game objects to their parent
	for (auto pending : SceneGraphNode::pendingChildren) {

		// Add the pending gameObject to the parent's child list
		pending->sceneNode.parent->children.emplace_back(pending);
	}
	SceneGraphNode::pendingChildren.clear();

	// Delete dead game objects
	for (auto gameObject : SceneGraphNode::deadGameObjects) {

		// Delete the dead game object from the parent's child list
		gameObject->sceneNode.parent->removeGameObjectFromSceneGraph(gameObject);

		delete gameObject;
	}

	// Clear the list for the next update cycle
	SceneGraphNode::deadGameObjects.clear();

} // end updateSceneGraph