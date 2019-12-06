#pragma once
#include "LightComponent.h"
class PositionalLightComponent :public LightComponent
{
public:
	PositionalLightComponent(lightSource id, bool enabled);

	virtual void update(float deltaTime) override;
};

