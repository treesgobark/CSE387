#pragma once
#include "Component.h"
#include "MathLibsConstsFuncs.h"
#include "SoundEngine.h"

class SoundBaseComponent : public Component
{
	public:

	SoundBaseComponent( int updateOrder = 500 );

	/**
	 * @fn	virtual void SoundBaseComponent::update( float deltaTime ) override;
	 *
	 * @brief	Performs update calculations are are common to all SoundBase sub-classes i.e. the
	 * 			data members of this class based on the world tranformation of the SceneNode.
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update( float deltaTime ) override;

	protected:

	FMOD_VECTOR fmod_pos = { 0, 0, 0 };
	FMOD_VECTOR fmod_vel = { 0, 0, 0 };
	FMOD_VECTOR fmod_forward = { 0, 0, 0 };
	FMOD_VECTOR fmod_up = { 0, 0, 0 };

};

