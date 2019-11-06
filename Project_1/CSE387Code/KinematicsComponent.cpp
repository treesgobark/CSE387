#include "KinematicsComponent.h"

KinematicsComponent::KinematicsComponent(int updateOrder)
	:Component(updateOrder)
{

}

void KinematicsComponent::update(float deltaTime)
{
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
	owningGameObject->sceneNode.setPosition(position, LOCAL);
}
