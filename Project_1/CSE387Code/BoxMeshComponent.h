#pragma once
#include "MeshComponent.H"


class BoxMeshComponent : public MeshComponent
{
	public:
	BoxMeshComponent(Material* material, float width = 1.0f, float height = 1.0f, float depth = 1.0f, int updateOrder = 100);

	virtual void initialize() override;
;

	protected:

	float halfWidth, halfHeight, halfDepth;

	Material * material;

};

