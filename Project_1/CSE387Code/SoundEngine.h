#pragma once

#include "MathLibsConstsFuncs.h"

// Includes for the FMOD Sound Engine
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

class SoundEngine 
{
public:

	/**
	 * @fn	static bool SoundEngine::Init();
	 *
	 * @brief	Initialize the sound engine.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	static bool Init();

	/**
	 * @fn	static void SoundEngine::Update(const float & deltaTime = 0.0f);
	 *
	 * @brief	Update the sound engine. Call this once each frame.
	 *
	 * @param	deltaTime	(Optional) The delta time.
	 */
	static void Update(const float & deltaTime = 0.0f);

	/**
	 * @fn	static void SoundEngine::Stop();
	 *
	 * @brief	Stop the sound engine. Call when closing down
	 */
	static void Stop();

	friend class SoundBaseComponent;
	friend class SoundListenerComponent;
	friend class SoundSourceComponent;
	friend class SoundReverbZoneComponent;

protected:

	/**
	 * @fn	static void SoundEngine::HandleError(FMOD_RESULT result);
	 *
	 * @brief	Call to display errors from sound engine functions.
	 *
	 * @param	result	The result.
	 */
	static void HandleError(FMOD_RESULT result);

	/** @brief	The main system object for FMOD Studio. */
	static FMOD::System* system;
};

