#include "Component.h"

Component::Component(class GameObject* owner, int updateOrder)
	:owner(owner)
	,updateOrder(updateOrder)
{
	// Add to GameObject's vector of components
	owner->AddComponent(this);
}

void Component::Initialize()
{

}

Component::~Component()
{
	owner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}
