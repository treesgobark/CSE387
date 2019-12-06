#pragma once
#include "LightComponent.h"
class DirectionalLightComponent :public LightComponent
{
public:
	DirectionalLightComponent(lightSource id, bool enabled);

	virtual void update(float deltaTime) override;
};

