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
	sceneNode.getParent()->removeGameObjectFromSceneGraph(this);

	// Need to delete components
	while (!components.empty()) {

		// Check if this Component is a Mesh 
		if (components.back()->getComponentType() == MESH) {

			this->owningGame->removeMeshComp(dynamic_cast<MeshComponent*>(components.back()));
		}

		delete components.back();
	}

	// Delete all children
	for (GameObject* child : this->sceneNode.getChildren()) {

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
	for (auto child : sceneNode.getChildren()) {
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
		for (auto child : this->sceneNode.getChildren()) {
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
		for (auto child : this->sceneNode.getChildren()) {
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
	if (component->getComponentType() == MESH) {

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

	// Initialize only if the gameObject is already in the
	// scene graph and the game has been initialized.
	if (this->sceneNode.getParent() != nullptr && 
		owningGame->getGameInitializationComplete() == true) {

		// Adding a new component to a game object that is already in 
		// the scene graph and initialized.
		component->initialize();
	}

} // end addComponent


void GameObject::removeComponent(Component* component)
{
	// Find the component to be removed
	auto iter = std::find(components.begin(), components.end(), component);

	// Erase the component if it was found
	if (iter != components.end()) {
		components.erase(iter);

		if (component->getComponentType() == MESH) {

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
		SceneGraphNode::designateDeadGameObject(this);

	}
}

void GameObject::addChild(GameObject* child)
{
	this->sceneNode.addGameObjectToSceneGraph(child);

	if (getOwningGame()->getGameInitializationComplete() == true) {

		child->initialize();

	}
}

void GameObject::removeChild(GameObject* child)
{
	this->sceneNode.removeGameObjectFromSceneGraph(child);

}

void GameObject::removeAndDeleteChild(GameObject* child)
{
	this->sceneNode.removeGameObjectFromSceneGraph(child);

	child->setState(DEAD);

}
