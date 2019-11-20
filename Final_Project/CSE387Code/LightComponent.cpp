#include "LightComponent.h"
#include "GameObject.h"
#include "Game.h"

LightComponent::LightComponent(lightSource id, bool enabled) 
	: id(id)
{
	SharedGeneralLighting::setEnabled(id, enabled);
}


void LightComponent::setEnable()
{
	SharedGeneralLighting::setEnabled(id, true);
}
void LightComponent::setDisable()
{
	SharedGeneralLighting::setEnabled(id, false);
}

void LightComponent::setAmbientColor(vec4 newAmb)
{
	SharedGeneralLighting::setAmbientColor(id, newAmb);
}

void LightComponent::setDiffuseColor(vec4 newDif)
{
	SharedGeneralLighting::setDiffuseColor(id, newDif);
}

void LightComponent::setSpecularColor(vec4 newSpec)
{
	SharedGeneralLighting::setSpecularColor(id, newSpec);
}
