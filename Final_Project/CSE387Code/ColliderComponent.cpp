#include "ColliderComponent.h"
#include "MathLibsConstsFuncs.h"
#include "RigidBodyComponent.h"
#include "MeshComponent.h"


ColliderComponent::ColliderComponent(class GameObject* owner) :
	Component(owner)//, mRadius(0.0f)
{
	mComponentName = "collider";
	mOwner->mColliderComp = this;
}

void ColliderComponent::Initialize()
{
	Component::Initialize();
	MeshComponent *tempMesh = (MeshComponent*)mOwner->getComponentByType("MeshComponent");
	//MeshComponent *tempMesh = mOwner->mMeshComp;
	if (tempMesh != nullptr) {
		collisionShape = tempMesh->GetCollisionShape();
	}
	else 
	{
		cout << "Collider Component can not find Mesh Component" << endl;
		mOwner->GetGame()->Shutdown();
	}
}
ColliderComponent::~ColliderComponent()
{
	delete collisionShape;
}
