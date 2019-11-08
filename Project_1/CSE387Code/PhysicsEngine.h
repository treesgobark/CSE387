#pragma once

#include "btBulletDynamicsCommon.h"
#include "MathLibsConstsFuncs.h"
#include "RigidBodyComponent.h"

#include <set>			// For holding sets or collision pairs
#include <algorithm>    // std::set_difference, std::sort
#include <iterator>		// std::inserter

// typedefs for collision events
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;


class PhysicsEngine
{
	public:

	/**
	 * Initialize the engine.
	 */
	static bool Init();

	/**
	 * Update the engine. Call this once each frame.
	 * @param deltaTime - time in seconds since the last update.
	 */
	static void Update(const float & deltaTime = 0.0f);

	/**
	 * Stop the engine. Call when closing down.
	 */
	static void Stop();

	/**
	 * Generates collision events that are passed to the game engine.
	 * @param world - Contains the physics simulation
	 * @param timeStep - incremental time step in the update.
	 */
	static void collisionEventCallback(btDynamicsWorld *world, btScalar timeStep);

	/**
	 * Passes events to SceneNodes in the Game Engine at the beginning of a collision.
	 * @param rigBody0 - first rigid body involved in the collision.
	 * @param rigBody1 - seconde rigid body involved in the collision.
	 */
	static void CollisionEnter(const btRigidBody * rigBody0, const btRigidBody * rigBody1);

	/**
	 * Passes events to SceneNodes in the Game Engine at the end of a collision.
 	 * @param rigBody0 - first rigid body involved in the collision.
	 * @param rigBody1 - seconde rigid body involved in the collision.
	 */
	static void CollisionExit(const btRigidBody * rigBody0, const btRigidBody * rigBody1);
	
	/**
	 * Converts a glm::mat4 into a btTransform.
	 * @param trans - mat4 to be converted to a btTransform
	 * @return transformation expressed as a btTransform
	 */
	static btTransform convertTransform(const glm::mat4 & trans);

	/**
	* Converts a btTransform into a glm::mat4.
	* @param trans - btTransform to be converted to a glm::mat4
	* @return transformation expressed as a glm::mat4
	*/
	static glm::mat4 convertTransform(const btTransform & trans);

	// See Init method for purpose of each of these objects.
	static btBroadphaseInterface* broadphase;
	static btDefaultCollisionConfiguration* collisionConfiguration;
	static btCollisionDispatcher* dispatcher;
	static btSequentialImpulseConstraintSolver* solver;
	static btDiscreteDynamicsWorld* dynamicsWorld;

	// Collision event pairs from the last update
	static CollisionPairs colPairsLastUpdate;

};

