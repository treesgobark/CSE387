#//include <iostream>

#include "SoundEngine.h"


FMOD::System* SoundEngine::system = nullptr;

bool SoundEngine::Init()
{
	if (SoundEngine::system == nullptr) {

		FMOD_RESULT result;

		// Create the main object for the FMOD Low Level System.
		result = FMOD::System_Create(&system);      
		if (result != FMOD_OK)
		{
			SoundEngine::HandleError(result);
			return false;
		}

		/*
		 By default FMOD uses a left-handed coordinate system. To use a right-handed 
		 coordinate system FMOD must be initialized by passing FMOD_INIT_3D_RIGHTHANDED 
		 to System::init. 

		 FMOD requires that the positive Y axis is up and the positive X axis 
		 is right, using a different convention requires and an additional must rotation 
		 of vectors into FMOD's space.
		*/ 
		result = system->init(512, FMOD_INIT_3D_RIGHTHANDED, 0);   
		if (result != FMOD_OK)
		{
			SoundEngine::HandleError(result);
			return false;
		}
	}

	std::cout << "******** FMOD Low Level API successfully initiliazed. **************" << std::endl;

	/*
	 * Set the global doppler scale, distance factor and log rolloff scale for all 3D sound in FMOD.
	 * dopplerscale - Scaling factor for doppler shift. Default = 1.0.
	 * distancefactor - Relative distance factor to FMOD's units. Default = 1.0. (1.0 = 1 metre).
	 *                  equates to "how many units per meter does your engine have". For example, 
	 *                  if you are using feet then "scale" would equal 3.28.
	 * rolloffscale - Scaling factor for 3D sound rolloff or attenuation for FMOD_3D_INVERSEROLLOFF 
	 *                based sounds only (which is the default type). Default = 1.0.
	*/
	SoundEngine::HandleError(SoundEngine::system->set3DSettings(1.0f, 1.0f, 1.0f));

	return true;

} // end SoundEngineInit


void SoundEngine::Update(const float & deltaTime)
{
	/*
	* Updates the FMOD system. This should be called once per 'game' tick, or once per frame.
	* Gets 3D positioning for 3D sound.
	*/
	SoundEngine::HandleError(SoundEngine::system->update());
}

void SoundEngine::Stop()
{
	std::cout << "******** FMOD Shut Down. **************" << std::endl;
	/*
	 * Closes and frees a system object and its resources 
	 */
	SoundEngine::HandleError(SoundEngine::system->release());

}

void SoundEngine::HandleError(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}

} // end HandleError
