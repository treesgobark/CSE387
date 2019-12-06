#include "PositionalLightComponent.h"

PositionalLightComponent::PositionalLightComponent(lightSource id, bool enabled) :
	LightComponent(id, enabled)
{

}

void PositionalLightComponent::update(float deltaTime)
{
	SharedGeneralLighting::setPositionOrDirection(id, vec4(owningGameObject->sceneNode.getFowardDirection(), 1.0f));
}
