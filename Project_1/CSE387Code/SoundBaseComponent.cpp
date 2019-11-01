#include "SoundBaseComponent.h"
#include "Game.h"
#include "GameObject.h"

SoundBaseComponent::SoundBaseComponent( int updateOrder)
	:Component( updateOrder )
{
}

void SoundBaseComponent::update( float deltaTime )
{
	// Get current position
	vec3 position = this->owningGameObject->sceneNode.getPosition( WORLD );
	vec3 foward = this->owningGameObject->sceneNode.getFowardDirection( WORLD );
	vec3 up = this->owningGameObject->sceneNode.getUpDirection( WORLD );

	// Previous position used for velocity calculations
	static vec3 lastPosition = position;

	// Calculate the velocity 
	vec3 velocity = vec3( 0, 0, 0 );

	if( deltaTime > 0.0f ) {
		velocity = ( ( position - lastPosition ) ) / ( 1000 * deltaTime );
	}

	// Save position for next update
	lastPosition = position;

	fmod_pos = { position.x, position.y, position.z };
	fmod_vel = { velocity.x, velocity.y, velocity.z };
	fmod_forward = { foward.x, foward.y, foward.z };
	fmod_up = { up.x, up.y, up.z };

} // end soundUpdate

