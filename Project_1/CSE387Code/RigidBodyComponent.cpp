#include "RigidBodyComponent.h"
#include "MeshComponent.h"
//#include "ColliderComponent.h"


RigidBodyComponent::RigidBodyComponent(MeshComponent* meshComponent, DynamicsState state, int updateOrder)
	: Component( updateOrder), meshComponent(meshComponent), rigidbodyDynamics(state)
{

}

void RigidBodyComponent::CollisionExit(const RigidBodyComponent* collisionData) const
{
}

void RigidBodyComponent::initialize()
{
	// Get the collision shape for the mesh component after the mesh component has been
	// initialized. If does not exist until after this has been completed.
	// this->bulletCollisionShape = meshComponent->getCollisionShape();

	// Get the scale value to use in scaling the mesh.
	glm::vec3 scale = getScaleFromTransform(this->owningGameObject->sceneNode.getScale(WORLD));

	// Set the scale for the collision shape
	this->bulletCollisionShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

	if (this->rigidbodyDynamics == DYNAMIC) {

		// Object staring with no inertia.
		btScalar mass = 1;
		btVector3 inertia(0, 0, 0);
		bulletCollisionShape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,							// mass SHOULD BE NON-ZERO FOR DYNAMIC RIGIDBODIES
			this,							// motion state
			bulletCollisionShape,			// collision shape of body
			inertia							// local inertia
		);

		this->bulletRigidBody = new btRigidBody(rigidBodyCI);

		PhysicsEngine::dynamicsWorld->addRigidBody(bulletRigidBody);
	}
	else if (this->rigidbodyDynamics == KINEMATIC_MOVING || this->rigidbodyDynamics == KINEMATIC_STATIONARY) {

		// Static object with no inertia.
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,							// mass SHOULD BE ZERO FOR KINEMATIC RIGIDBODIES
			this,						// motion state
			bulletCollisionShape,		// collision shape of body
			btVector3(0, 0, 0)			// local inertia
		);

		this->bulletRigidBody = new btRigidBody(rigidBodyCI);

		if (this->rigidbodyDynamics == KINEMATIC_MOVING) {
			bulletRigidBody->setCollisionFlags(bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			bulletRigidBody->setActivationState(DISABLE_DEACTIVATION);
		}
		else {

			bulletRigidBody->setCollisionFlags(bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		}

		PhysicsEngine::dynamicsWorld->addRigidBody(bulletRigidBody);
	}
	else {
		this->bulletRigidBody = nullptr;
		this->bulletCollisionShape = nullptr;
	}
}

RigidBodyComponent::~RigidBodyComponent()
{
	//mOwner->RemoveComponent(this);
	if (bulletRigidBody != nullptr && PhysicsEngine::dynamicsWorld != nullptr) {
		PhysicsEngine::dynamicsWorld->removeRigidBody(bulletRigidBody);
		//_aligned_free(bulletRigidBody->getMotionState()); // May be an error source on shutdown
	}
}

void RigidBodyComponent::setVelocity(vec3 worldVelocity)
{
	bulletRigidBody->setLinearVelocity(btVector3(worldVelocity.x, worldVelocity.y, worldVelocity.z));

}

void RigidBodyComponent::setAngularVelocity(vec3 worldAngVelocity)
{
	bulletRigidBody->setAngularVelocity(btVector3(worldAngVelocity.x, worldAngVelocity.y, worldAngVelocity.z));
}

// Called by bullet to get the position and orientation of the object
void RigidBodyComponent::getWorldTransform(btTransform &worldTrans) const
{
	// Calculate current world transformation and set world trans to it 
	worldTrans = PhysicsEngine::convertTransform( this->owningGameObject->sceneNode.getTransformation(WORLD));

	//std::cout << "RigidBodyComponent::getWorldTransform" << std::endl;
}

// Callled by bullet to set the position and orient the object
void RigidBodyComponent::setWorldTransform(const btTransform &worldTrans)
{
	bulletTransform = worldTrans;

	glm::mat4 T = PhysicsEngine::convertTransform(worldTrans);

	this->owningGameObject->sceneNode.setTransform(T, WORLD);

	//std::cout << "RigidBodyComponent::setWorldTransform" << std::endl;
}



void RigidBodyComponent::CollisionEnter(const RigidBodyComponent* collisionData) const
{
	std::vector<class Component*> buddies = owningGameObject->getComponents();
	
	for (auto bud : buddies) {
		// bud->CollisionEnter(collisionData);
	}
}

