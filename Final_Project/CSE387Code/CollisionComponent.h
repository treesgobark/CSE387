#pragma once

#include "RigidBodyComponent.h"

class CollisionComponent : public Component
{
public:

	virtual COMPONENT_TYPE getComponentType() override { return COLLISION; }

	/**
	 * @fn	void CollisionComponent::CollisionEnter( const RigidBodyComponent * collisionData ) const
	 *
	 * @brief	Method is called whenever a rigid body attached to the same GameObject first collides 
	 * 			with another rigid body.
	 *
	 * @param	collisionData	Information describing the collision.
	 */
	virtual void CollisionEnter(const RigidBodyComponent* collisionData) {};

	/**
	 * @fn	void CollisionComponent::CollisionExit( const RigidBodyComponent * collisionData ) const
	 *
	 * @brief	Called whenever this rigid body attached to the same GameObject exits a collision 
	 * 			with another rigid body.
	 *
	 * @param	collisionData	Information describing the collision.
	 */
	virtual void CollisionExit(const RigidBodyComponent* collisionData) {};

};

