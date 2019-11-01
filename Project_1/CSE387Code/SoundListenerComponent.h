#pragma once
#include "SoundBaseComponent.h"

class SoundListenerComponent : public SoundBaseComponent
{
public:

	SoundListenerComponent( int updateOrder = 500);

	/**
	 * @fn	virtual void SoundListenerComponent::update( float deltaTime ) override;
	 *
	 * @brief	Updates the position, orientation, and up and forward directions of the listener
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update( float deltaTime ) override;
};

