#include "SoundReverbZoneComponent.h"

#include "GameObject.h"



SoundReverbZoneComponent::SoundReverbZoneComponent( float mindist, float maxdist, int updateOrder)
	:SoundBaseComponent( updateOrder ),mindist( mindist ), maxdist( maxdist )
{
	FMOD_RESULT result = SoundEngine::system->createReverb3D(&reverb);

	if (result == FMOD_OK)
	{
		FMOD_REVERB_PROPERTIES prop2 = FMOD_PRESET_CONCERTHALL;
		SoundEngine::HandleError(reverb->setProperties(&prop2));

		FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
		SoundEngine::HandleError(reverb->set3DAttributes(&pos, mindist, maxdist));
	}
	else {
		SoundEngine::HandleError(result);
	}
}


void SoundReverbZoneComponent::update( float deltaTime )
{
	// Calculate the World position and orientation 
	SoundBaseComponent::update(deltaTime);

	SoundEngine::HandleError(reverb->set3DAttributes(&fmod_pos, mindist, maxdist));
}
