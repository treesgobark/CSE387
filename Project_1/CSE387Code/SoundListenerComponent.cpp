#include "SoundListenerComponent.h"


SoundListenerComponent::SoundListenerComponent( int updateOrder )
	:SoundBaseComponent( updateOrder )
{
}


void SoundListenerComponent::update( float deltaTime )
{
	/**
	Velocity is only required if you want doppler effects. Otherwise you can
	pass 0 or nullptr to both System::set3DListenerAttributes and Channel::set3DAttributes
	for the velocity parameter, and no doppler effect will be heard.

	The velocity passed to FMOD Studio is meters per second and not meters per frame.
	*/

	// Calculate the World position and orientation 
	SoundBaseComponent::update( deltaTime );

	SoundEngine::HandleError( SoundEngine::system->set3DListenerAttributes(
		0, &fmod_pos, &fmod_vel, &fmod_forward, &fmod_up ) );

} // end soundUpdate
