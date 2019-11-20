#include "PhysicsEngine.h"

#include "RigidBodyComponent.h"

#define VERBOSE false

// Static Data Member Definitions
btBroadphaseInterface* PhysicsEngine::broadphase;
btDefaultCollisionConfiguration* PhysicsEngine::collisionConfiguration;
btCollisionDispatcher* PhysicsEngine::dispatcher;
btSequentialImpulseConstraintSolver* PhysicsEngine::solver;
btDiscreteDynamicsWorld* PhysicsEngine::dynamicsWorld;

// Set containing the collision pairs from the last update
CollisionPairs PhysicsEngine::colPairsLastUpdate;

bool PhysicsEngine::Init()
{
	// Bullet uses a right-handed coordinate system.
	// btScalar is a floating point number
	// btVector3 has 3 scalar x,y,z components and a 4th unused w component
	// 3D orientations and rotations can be represented using either btQuaternion or btMatrix3x3

	/*
	  The broadphase algorithm usually uses the bounding boxes of objects in the 
	  world to quickly compute a conservative approximate list of colliding 
	  pairs. The list will include every pair of objects that are colliding, 
	  but it may also include pairs of objects whose bounding boxes intersect 
	  but are still not close enough to collide. Later stages will calculate 
	  exact collisions, and thus these extra pairs will be eliminated, but 
	  those algorithms are much slower so we don't want to run them on every 
	  pair of rigid bodies in the world.

	  btDbvtBroadphase implements a broadphase using two dynamic AABB 
	  bounding volume hierarchies/trees.

	  See http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Broadphase
	*/
	broadphase = new btDbvtBroadphase();

	/*
	Set algorithms used for narrow phase collision detection to default.
	*/
	collisionConfiguration = new btDefaultCollisionConfiguration();

	/*
	btCollisionDispatcher supports algorithms that handle ConvexConvex 
	and ConvexConcave collision pairs
	*/
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	/*
	This is what causes the objects to interact properly, taking 
	into account gravity, game logic supplied forces, collisions, and 
	hinge constraints.
	
	The btSequentialImpulseConstraintSolver is a fast SIMD implementation 
	of the Projected Gauss Seidel (iterative LCP) method. 
	*/
	solver = new btSequentialImpulseConstraintSolver;

	/*
	Entire physics pipeline computation and its data structures are represented in Bullet by a
	dynamics world. Create a btDiscreteDynamicsWorld or btSoftRigidDynamicsWorld.
	*/
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	// Set gravity direction in the dynamics world.
	dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

	// Register the collision event generator. The register function will be called once each time
	// the physics engine updates. 
	dynamicsWorld->setInternalTickCallback(PhysicsEngine::collisionEventCallback, nullptr, false);

	if (VERBOSE) std::cout << "******** Bullet Physics Engine successfully initiliazed. **************" << std::endl;

	return true;

} // end Init


void PhysicsEngine::Update(const float & deltaTime)
{
	// Update the simulation by deltaTime on each update. Automatically takes into account 
	// variability in deltaTime performing interpolation up to 10 interpolation steps.
	// Each internal interpolation step will be a 60th of a second. deltaTime should
	// always be less than 10 * (1/60).
	dynamicsWorld->stepSimulation(deltaTime, 10, btScalar(1 / 60.0f));

} // end Update


void PhysicsEngine::Stop()
{
	/* Smart pointers are connected to some objects in the physics engine.
	* Deleting them here causes errors on shutdown. Probably because
	* rigid bodies and shapes have already been deleted by smart points.
	*/
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	if (VERBOSE) std::cout << "******** Bullet Physics Engine Shut Down. **************" << std::endl;

} // end Stop


void PhysicsEngine::collisionEventCallback(btDynamicsWorld *world, btScalar timeStep)
{
	// Set of collision pairs found during the current update
	CollisionPairs colPairsThisUpdate;

	// Iterate through all of the manifolds in the dispatcher
	// A contact manifold is a cache that contains all contact points between a pair of 
	// collision objects.
	int numManifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++)
	{
		// Get the manifold
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);

		// Ignore manifolds that have no contact points.
		if (contactManifold->getNumContacts() > 0)
		{
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(contactManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(contactManifold->getBody1());

			// Order the pair using the pointer value.
			bool const swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			// Create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			// insert the pair into the current list
			colPairsThisUpdate.insert(thisPair);

			// If the pair is new one, generate a collision event 
			if (colPairsLastUpdate.find(thisPair) == colPairsLastUpdate.end())
			{
				CollisionEnter((btRigidBody*)pBody0, (btRigidBody*)pBody1);
			}
		}
	}

	// Set of pairs removed during this update
	CollisionPairs removedPairs;

	// Find the difference between gets colPairsLastUpdate colPairsThisUpdate
	// and push the difference into removedPars
	std::set_difference( colPairsLastUpdate.begin(), colPairsLastUpdate.end(),
						 colPairsThisUpdate.begin(), colPairsThisUpdate.end(),
						std::inserter(removedPairs, removedPairs.begin() ) );

	// Generate separation events for removed pairs
	for (CollisionPairs::const_iterator iter = removedPairs.begin();
		iter != removedPairs.end(); ++iter)
	{
		CollisionExit((btRigidBody*)iter->first, (btRigidBody*)iter->second);
	}

	// Save set of collision pairs for next update
	colPairsLastUpdate = colPairsThisUpdate;

} // end collisionEventCallback


void PhysicsEngine::CollisionEnter(const btRigidBody * rigBody0, const btRigidBody * rigBody1)
{
	const RigidBodyComponent* moState0 = static_cast<const RigidBodyComponent*>( rigBody0->getMotionState( ) );
	const RigidBodyComponent* moState1 = static_cast<const RigidBodyComponent*>( rigBody1->getMotionState( ) );

	moState0->CollisionEnter(moState1);
	moState1->CollisionEnter(moState0);

} // end CollisionEnter

void PhysicsEngine::CollisionExit(const btRigidBody * rigBody0, const btRigidBody * rigBody1)
{
	const RigidBodyComponent* moState0 = static_cast<const RigidBodyComponent*>( rigBody0->getMotionState( ) );
	const RigidBodyComponent* moState1 = static_cast<const RigidBodyComponent*>( rigBody1->getMotionState( ) );

	moState0->CollisionExit( moState1 );
	moState1->CollisionExit( moState0 );

} // end CollisionExit


// Converts a glm::mat4 into a btTransform
btTransform PhysicsEngine::convertTransform(const glm::mat4 & trans)
{
	btVector3 origin(trans[3][0], trans[3][1],trans[3][2]);

	// Row Major
	btMatrix3x3 rot(trans[0][0], trans[1][0], trans[2][0],
					trans[0][1], trans[1][1], trans[2][1],
					trans[0][2], trans[1][2], trans[2][2]);

	btTransform btTrans(rot, origin);

	return btTrans;

} // end convertTransform


// Converts a btTransform into a glm::mat4
glm::mat4 PhysicsEngine::convertTransform(const btTransform & trans)
{
	btMatrix3x3 rot = trans.getBasis();
	
	btVector3 c1 = rot.getColumn(0);
	btVector3 c2 = rot.getColumn(1);
	btVector3 c3 = rot.getColumn(2);
	btVector3 c4 = trans.getOrigin();

	glm::mat4 glmTrans( c1[0], c1[1], c1[2], 0.0f,
						c2[0], c2[1], c2[2], 0.0f,
						c3[0], c3[1], c3[2], 0.0f,
						c4[0], c4[1], c4[2], 1.0f );

	return glmTrans;

} // end convertTransform

