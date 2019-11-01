#include "GameObject.h"

#include "Component.h"
#include "MeshComponent.h"

#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

GameObject::GameObject(Game * game)
	: owningGame(game),   gameObjectState( ACTIVE )
{


} // end GameObject constructor

GameObject::~GameObject()
{
	// Remove the game object from the game
	sceneNode.parent->removeChild(this);

	// Need to delete components
	while (!components.empty()) {

			delete components.back();
	}

	// Delete all children
	for (GameObject* child : sceneNode.children) {

		delete child;
	}

} // end GameObject destructor


void GameObject::initialize()
{
	// Initialize the components that are attached to this game object
	for (auto component : components) {

		component->initialize();
	}

	// Initialize all children
	for (auto child : sceneNode.children) {
		child->initialize();
	}

} // end initialize


void GameObject::update(float deltaTime)
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// Update the components that are attached to to this game object
		for (auto component : this->components) {

			component->update(deltaTime);
		}

		// Update this game object
		updateGameObject(deltaTime);

		// Update all children
		for (auto child : this->sceneNode.children) {
			child->update(deltaTime);
		}
	}

} // end update


void GameObject::updateGameObject(float deltaTime)
{

} // end updateGameObject

void GameObject::processInput()
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// First process input for components
		for (auto comp : this->components) {
			comp->processInput();
		}

		// Process input for this game object
		gameObjectInput();

		// Process input for children
		for (auto child : this->sceneNode.children) {
			child->processInput();
		}
	}

} // end processInput

void GameObject::gameObjectInput()
{
	// Override to create custom input response


} // end gameObjectInput


void GameObject::addComponent(Component* component)
{
	// Dependency injection
	component->owningGameObject = this;

	// Check if this Component is a Mesh
	if (component->isMesh() == true) {

		this->owningGame->addMeshComp(dynamic_cast<MeshComponent *>(component));
	}

	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();

	auto iter = components.begin();
	for (; iter != components.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()) {
			break;
		}
	}

	// Inserts element before position of iterator
	components.insert(iter, component);

} // end addComponent


void GameObject::removeComponent(Component* component)
{
	// Find the component to be removed
	auto iter = std::find(components.begin(), components.end(), component);

	// Erase the component if it was found
	if (iter != components.end()) {
		components.erase(iter);

		if (component->isMesh() == true) {

			this->owningGame->removeMeshComp(dynamic_cast<MeshComponent*>(component));
		}
	}

} // end removeComponent


void GameObject::setState(STATE state)
{
	gameObjectState = state;

	if (gameObjectState == DEAD) {

		// Add the this dead game object to static list of game objects
		// to be deleted on the next update cycle.
		SceneNode::deadGameObjects.emplace_back(this);

	}
}
