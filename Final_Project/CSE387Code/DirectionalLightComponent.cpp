#include "DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(lightSource id, bool enabled) :
	LightComponent(id, enabled)
{

}

void DirectionalLightComponent::update(float deltaTime)
{
	SharedGeneralLighting::setPositionOrDirection(id, vec4(owningGameObject->sceneNode.getFowardDirection(), 0.0f));
}
