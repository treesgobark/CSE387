#include "Component.h"

Component::Component( int updateOrder)
	: updateOrder(updateOrder)
{

}

void Component::initialize()
{

}

Component::~Component()
{
	owningGameObject->removeComponent(this);
}

void Component::update(float deltaTime)
{
}
