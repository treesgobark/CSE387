#include "GameObject.h"
#include "Component.h"
#include <algorithm>
#include <glm/gtx/matrix_decompose.hpp>


GameObject::GameObject(Game* game)
	:gameObjectState(EActive)
{

}

GameObject::~GameObject()
{
	//game->RemoveGameObject(this);

	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!components.empty()) {
		delete components.back();
	}
}

void GameObject::Update(float deltaTime)
{
	if (gameObjectState == EActive) {

		UpdateComponents(deltaTime);
		UpdateGameObject(deltaTime);

	}
}

void GameObject::UpdateComponents(float deltaTime)
{
	for (auto comp : components) {
		comp->Update(deltaTime);
	}

}

void GameObject::UpdateGameObject(float deltaTime)
{
}

void GameObject::ProcessInput()
{
	if (gameObjectState == EActive) {

		// First process input for components
		for (auto comp : components) {
			comp->ProcessInput();
		}

		GameObjectInput();
	}
}

void GameObject::GameObjectInput()
{
}

void GameObject::AddComponent(Component* component)
{
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
}

void GameObject::RemoveComponent(Component* component)
{
	auto iter = std::find(components.begin(), components.end(), component);
	if (iter != components.end()) {
		components.erase(iter);
	}
}




