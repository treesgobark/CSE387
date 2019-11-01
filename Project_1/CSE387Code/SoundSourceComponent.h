#pragma once
#include "SoundBaseComponent.h"
#include <string>

/**
 * @class	SoundSourceComponent
 *
 * @brief	Default 3D distance model is FMOD_3D_INVERSEROLLOFF
 * 			
 * 			When FMOD uses this model, 'mindistance' of a sound / channel, is the distance that
 * 			the sound starts to attenuate from.  This can simulate the sound being smaller or
 * 			larger. By default, for every doubling of this mindistance, the sound volume will
 * 			halve.
 * 			
 * 			The 'maxdistance' does not affect the rate of rolloff, it simply means the distance
 * 			where the sound stops attenuating. Don't set the maxdistance to a low number unless
 * 			you want it to artificially stop attenuating. This is usually not wanted. Leave it at
 * 			its default of 10000.0.
 * 			
 * 			If the 3D distance model is set to FMOD_3D_LINEARSQUAREDROLLOFF flag using
 * 			System::createSound or Sound::setMode,  the 'mindistance' and 'maxdistance' settings
 * 			to change the attenuation behaviour to fading linearly between the two distances.
 * 			Below the mindistance is the sound is at full volume. Beyond the the minimum distance
 * 			the sound starts to attenuate). At the maxdistance the volume = 0 due to 3D distance.
 * 			
 * 			The 3 main configurable settings in FMOD Studio that affect all 3D sounds are: *
 * 			Doppler factor. This is just a way to exaggerate or minimize the doppler effect. *
 * 			Distance factor. This allows the user to set FMOD to use units that match their own
 * 			(ie centimeters, meters, feet) * Rolloff
 * 			. Affects 3d sounds that use
 * 			FMOD_3D_LOGROLLOFF. Controls how fast all sounds attenuate using this mode. All 3
 * 			settings can be set with System::set3DSettings.
 */
class SoundSourceComponent : public SoundBaseComponent
{
public:

	/**
	 * @fn	SoundSourceComponent::SoundSourceComponent( std::string soundFileName, float mindist = 100.0f, float maxdist = 500.0f, FMOD_MODE fmod_mode = FMOD_3D_WORLDRELATIVE, int updateOrder = 500);
	 *
	 * @brief	Constructor
	 *
	 * @param	soundFileName	Filename of the sound file.
	 * @param	mindist		 	(Optional) The mindist.
	 * @param	maxdist		 	(Optional) The maxdist.
	 * @param	fmod_mode	 	(Optional) The fmod mode.
	 * @param	updateOrder  	(Optional) The update order.
	 */
	SoundSourceComponent( std::string soundFileName, float mindist = 100.0f, float maxdist = 500.0f, 
						  FMOD_MODE fmod_mode = FMOD_3D_WORLDRELATIVE, int updateOrder = 500);

	/**
	 * @fn	SoundSourceComponent::~SoundSourceComponent();
	 *
	 * @brief	Destructor
	 * 			
	 */
	~SoundSourceComponent();

	/**
	 * @fn	virtual void SoundSourceComponent::update( float deltaTime ) override;
	 *
	 * @brief	Updates the position and orientation of the source
	 *
	 * @param	deltaTime	The delta time.
	 */
	virtual void update( float deltaTime ) override;

	/**
	 * @fn	void SoundSourceComponent::play( bool loop = false );
	 *
	 * @brief	Start playing the sound.
	 *
	 * @param	loop	(Optional) - true to loop the sound. false to play it once.
	 */
	void play( bool loop = false );

	/**
	 * @fn	void SoundSourceComponent::stop();
	 *
	 * @brief	Stop playing the sound.
	 */
	void stop();

	/**
	 * @fn	void SoundSourceComponent::pause(bool paused);
	 *
	 * @brief	Start playing the sound.
	 *
	 * @param	paused	- true to pause the sound. false to unpause the sound.
	 */
	void pause(bool paused);

	/**
	 * @fn	bool SoundSourceComponent::isPlaying( );
	 *
	 * @brief	Query if this object is playing
	 *
	 * @returns	True if playing, false if not.
	 */
	bool isPlaying( );

	/**
	 * @fn	void SoundSourceComponent::setLooping(bool loopingOn);
	 *
	 * @brief	Set this sound to loop.
	 *
	 * @param	loopingOn	- true to loop the sound. false to play it once.
	 */
	void setLooping(bool loopingOn);

	/**
	 * @fn	void SoundSourceComponent::setVolume(float volume);
	 *
	 * @brief	Set the volume of the sound. Volume level can be below 0 to invert a signal and above
	 * 			1 to amplify the signal. Note that increasing the signal level too far may cause
	 * 			audible distortion.
	 *
	 * @param	volume	- volume level for the sound.
	 */
	void setVolume(float volume);

	/**
	 * @fn	float SoundSourceComponent::getVolume();
	 *
	 * @brief	Returns the current volume level for the sound.
	 * 			@returne - volume level for the sound.
	 *
	 * @returns	The volume.
	 */
	float getVolume();

protected:

	std::string soundFileName;
	float volume = 1.0f;

	FMOD::Sound *sound;
	FMOD::Channel *channel;
	FMOD_MODE fmod_mode;
	
	float mindist;
	float maxdist;

};

