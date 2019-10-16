#include "GameObject.h"

#include "Component.h"
#include "MeshComponent.h"

#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>

GameObject::GameObject(Game * game)
	: owningGame(game),   gameObjectState( ACTIVE )
{
	owningGame->addGameObject(this);

} // end GameObject constructor

GameObject::~GameObject()
{
	// Remove the game object from the game
	owningGame->removeGameObject(this);

	// Need to delete components
	while (!components.empty()) {

			delete components.back();
	}

} // end GameObject destructor


void GameObject::initialize()
{
	// Initialize the components that are attached to this game object
	for (auto component : components) {

		component->initialize();
	}

} // end initialize


void GameObject::update(float deltaTime)
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// Update the components that are attached to to this game object
		updateComponents(deltaTime);

		// Update this game object
		updateGameObject(deltaTime);

	}

} // end update


void GameObject::updateComponents(float deltaTime)
{
	// Update the components that are attached to to this game object
	for (auto component : components) {

		component->update(deltaTime);
	}

} // end updateComponents

void GameObject::updateGameObject(float deltaTime)
{

} // end updateGameObject

void GameObject::processInput()
{
	// Check to see if this game object is active
	if (gameObjectState == ACTIVE) {

		// First process input for components
		for (auto comp : components) {
			comp->processInput();
		}

		// Process input for this game object
		gameObjectInput();
	}

} // end processInput

void GameObject::gameObjectInput()
{

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
