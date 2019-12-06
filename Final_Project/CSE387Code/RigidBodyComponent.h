#pragma once
#include "Component.h"
//#include <iostream>

// Physics
#include "PhysicsEngine.h"
#include "MeshComponent.h"


class RigidBodyComponent : 	public Component, public btMotionState
{
	public:

	/**
	 * @fn	RigidBodyComponent::RigidBodyComponent(class MeshComponent* meshComponent, DynamicsState state = DYNAMIC, int updateOrder = 100);
	 *
	 * @brief	Constructor
	 *
	 * @param [in,out]	meshComponent	Pointer the mesh component that the collision shape will be based on. 
	 * @param 		  	state		 	(Optional) The state.
	 * @param 		  	updateOrder  	(Optional) The update order.
	 */
	RigidBodyComponent(class MeshComponent* meshComponent, DynamicsState state = DYNAMIC, int updateOrder = 100, btScalar mass = 1);

	/**
	 * @fn	virtual RigidBodyComponent::~RigidBodyComponent( );
	 *
	 * @brief	Destructor
	 */
	virtual ~RigidBodyComponent( );

	/**
	 * @fn	virtual void RigidBodyComponent::getWorldTransform( btTransform &worldTrans ) const;
	 *
	 * @brief	Called by bullet to get the position and orientation of the object. It is only called
	 * 			once by the physics engine. After that the engine assumes it is in control and that it
	 * 			knows where the object is. Method must be implemented in order to sub-class btMotionState
	 * 			
	 *
	 * @param [in,out]	worldTrans	The world transaction.
	 */
	virtual void getWorldTransform( btTransform &worldTrans ) const;

	/**
	 * @fn	virtual void RigidBodyComponent::setWorldTransform( const btTransform &worldTrans );
	 *
	 * @brief	Callled repeatedly by bullet to set the position and orientation of the object.
	 * 			Method must be implemented in order to sub-class btMotionState.
	 *
	 * @param	worldTrans	The world transaction.
	 */
	virtual void setWorldTransform( const btTransform &worldTrans );

	/**
	 * @fn	virtual void RigidBodyComponent::setVelocity( vec3 worldVelocity );
	 *
	 * @brief	Called to set a velocity for the game object in meters per second.
	 *
	 * @param	worldVelocity	The linear velocity relative to the World coordinate frame
	 */
	virtual void setVelocity( vec3 worldVelocity );

	/**
	 * @fn	virtual void RigidBodyComponent::setAngularVelocity( vec3 worldAngVelocity );
	 *
	 * @brief	Sets angular velocity. Direction of the vector determines the axis of 
	 * 			rotation. Length of the vector determines the rotational speed.
	 *
	 * @param	worldAngVelocity	The angular velocity relative to the World coordinate frame
	 */
	virtual void setAngularVelocity( vec3 worldAngVelocity );

	/**
	 * @fn	void RigidBodyComponent::CollisionEnter( const RigidBodyComponent * collisionData ) const
	 *
	 * @brief	Method is called whenever this rigid body first collides with another rigid body.
	 *
	 * @param	collisionData	Information describing the collision.
	 */
	void CollisionEnter(const RigidBodyComponent* collisionData) const;
	/**
	 * @fn	void RigidBodyComponent::CollisionExit( const RigidBodyComponent * collisionData ) const
	 *
	 * @brief	Called whenever this rigid body exits a collision with another rigid body.
	 *
	 * @param	collisionData	Information describing the collision.
	 */
	void CollisionExit(const RigidBodyComponent* collisionData) const;

	/**
	 * @fn	void RigidBodyComponent::initialize() override;
	 *
	 * @brief	Initializes this object based on the Dynamics start supplied to the constructor.
	 */
	void initialize() override;

	protected:

	/**
	 * @class	btRigidBody*
	 *
	 * @brief	The rigid body that bullet holds in its physics simulation.
	 */
	class btRigidBody * bulletRigidBody = nullptr;

	/**
	 * @class	btCollisionShape*
	 *
	 * @brief	A bullet collision shape that is based of a mesh. The collision shape is
	 * 			created by MeshComponent sub classes.
	 */
	class btCollisionShape *  bulletCollisionShape;

	/**
	 * @class	btTransform
	 *
	 * @brief	4x4 homogenous transformation for the rigid body
	 */
	class btTransform bulletTransform;

	/** @brief	The rigidbody dynamics state.  
	 *  NONE indicates that the object will be ignored by the physics engine.   
	 *  KINEMATIC_STATIONARY indicates that the object will not move, but that objects can collide with it
	 *  KINEMATIC_MOVING indicates that the object will be moved and updated by  by another component   
	 *  instead of the physics engine, but collisions will still be detected.  
	 *  DYNAMIC indicates that the object will be moved and updated by the physics engine.  
	 */
	DynamicsState rigidbodyDynamics = NONE;

	/** @brief	Reference to the mesh component with the collision shape for this rigid body. */
	MeshComponent* meshComponent;

	btScalar mass;
};

