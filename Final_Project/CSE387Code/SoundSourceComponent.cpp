#include "SoundSourceComponent.h"


SoundSourceComponent::SoundSourceComponent( std::string soundFileName, float mindist, float maxdist, FMOD_MODE fmod_mode, int updateOrder)
	: SoundBaseComponent( updateOrder ), soundFileName( soundFileName ), fmod_mode( fmod_mode ),
	mindist( mindist ), maxdist( maxdist )
{

	FMOD_RESULT result = SoundEngine::system->createSound( soundFileName.c_str( ), fmod_mode, 0, &sound );

	if( result == FMOD_OK ) {
		// Set the minimum and maximum distance for this sound
		SoundEngine::HandleError( sound->set3DMinMaxDistance( mindist, maxdist ) );

		//SoundEngine::HandleError(sound->setMode(FMOD_3D_LINEARROLLOFF));
		SoundEngine::HandleError( sound->setMode( FMOD_3D_LINEARSQUAREROLLOFF ) );

		// Determine the length of the sound
		//unsigned int length;
		//SoundEngine::HandleError(sound->getLength(&length, FMOD_TIMEUNIT_MS));

		//std::cout << soundFileName << " sound initialized. Length "
		//	<< length / 1000.0f << " seconds." << std::endl;
	}
	else {
		std::cerr << "Error initilizing " << soundFileName << " sound." << std::endl;
		SoundEngine::HandleError( result );
	}

} // SoundSource constructor


SoundSourceComponent::~SoundSourceComponent( )
{
	if( sound != nullptr && channel != nullptr ) {

		SoundEngine::HandleError( channel->stop( ) );
	}

} // end SoundSource destructor


  /**
  Velocity is only required if you want doppler effects. Otherwise you can
  pass 0 or nullptr to both System::set3DListenerAttributes and Channel::set3DAttributes
  for the velocity parameter, and no doppler effect will be heard.

  The velocity passed to FMOD Studio is meters per second and not meters per frame.


  */

void SoundSourceComponent::update( float deltaTime )
{
	if( channel != nullptr ) {

		// Calculate the World position and orientation 
		SoundBaseComponent::update( deltaTime );

		SoundEngine::HandleError(channel->set3DAttributes(&fmod_pos, &fmod_vel));
	}

} // end update

  // Start playing the sound
void SoundSourceComponent::play( bool loop )
{
	if( sound != nullptr ) {

		setLooping( loop );

		// Play the source
		SoundEngine::HandleError( SoundEngine::system->playSound( sound, 0, false, &channel ) );
		SoundEngine::HandleError( channel->setVolume( volume ) );
	}
	else {
		std::cout << "Sound for " << soundFileName << " not initialized." << std::endl;
	}

} // end play

// Stop the sound. 
void SoundSourceComponent::stop( )
{
	// Stop the source
	if( channel != nullptr ) {
		SoundEngine::HandleError( channel->stop( ) );
	}

} // end stop

  // Pause the sound. When played again, it will start from the point 
  // where is was paused.
void SoundSourceComponent::pause( bool paused )
{
	if( channel != nullptr ) {

		// Pause the source
		SoundEngine::HandleError( channel->setPaused( paused ) );
	}
} // end pause


  // Turn looping on and off
/*
When calling this setMode, it will only take effect when the sound is played
again with System::playSound.
*/
void SoundSourceComponent::setLooping( bool loopingOn )
{
	if( sound != nullptr ) {
		if( loopingOn ) {
			// turns looping on
			SoundEngine::HandleError( sound->setMode( FMOD_LOOP_NORMAL ) );
		}
		else {
			// turns looping off	
			SoundEngine::HandleError( sound->setMode( FMOD_LOOP_OFF ) );
		}
	}
} // end setLooping

  // Check to see if a sound it actively playing
bool SoundSourceComponent::isPlaying( )
{
	if( channel != nullptr ) {
		bool playing;
		SoundEngine::HandleError( channel->isPlaying( &playing ) );
		return playing;
	}
	else {
		return false;
	}

} // end isPlaying

void SoundSourceComponent::setVolume( float volume )
{
	this->volume = volume;
	if( channel != nullptr ) {
		SoundEngine::HandleError( channel->setVolume( this->volume ) );
	}

}

float SoundSourceComponent::getVolume( )
{
	return volume;
}
