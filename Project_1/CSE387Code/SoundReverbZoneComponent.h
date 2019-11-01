#pragma once

#include "SoundBaseComponent.h"

class SoundReverbZoneComponent : 	public SoundBaseComponent
{
public:
	SoundReverbZoneComponent( float mindist = 10.0f, float maxdist = 20.0f, int updateOrder = 500);

	virtual void update( float deltaTime ) override;

protected:
	FMOD::Reverb3D *reverb;
	float mindist;
	float maxdist;
};

