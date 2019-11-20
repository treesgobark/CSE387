#include "BoxMeshComponent.h"

BoxMeshComponent::BoxMeshComponent(Material* material, float width , float height , float depth, int updateOrder)
	: material(material), MeshComponent(updateOrder), halfWidth(width / 2), halfHeight(height / 2), halfDepth(depth / 2)
{

}


void BoxMeshComponent::initialize()
{
	std::vector<pntVertexData> vData;
	std::vector<unsigned int> indices;

	vec3 v1(-halfWidth, -halfHeight, halfDepth);
	vec3 v2(halfWidth, -halfHeight, halfDepth);
	vec3 v3(halfWidth, halfHeight, halfDepth);
	vec3 v4(-halfWidth, halfHeight, halfDepth);
	vec3 v5(-halfWidth, -halfHeight, -halfDepth);
	vec3 v6(halfWidth, -halfHeight, -halfDepth);
	vec3 v7(halfWidth, halfHeight, -halfDepth);
	vec3 v8(-halfWidth, halfHeight, -halfDepth);

	vec3 n1(0, 0, 1);
	vec3 n2(0, 0, -1);
	vec3 n3(1, 0, 0);
	vec3 n4(-1, 0, 0);
	vec3 n5(0, -1, 0);
	vec3 n6(0, 1, 0);

	vec2 t1(0, 1);
	vec2 t2(1, 1);
	vec2 t3(1, 0);
	vec2 t4(0, 0);

	// front face
	vData.push_back(pntVertexData(v1, n1, t4));
	vData.push_back(pntVertexData(v2, n1, t3));
	vData.push_back(pntVertexData(v3, n1, t2));
	vData.push_back(pntVertexData(v4, n1, t1));

	// right face
	vData.push_back(pntVertexData(v2, n3, t1));
	vData.push_back(pntVertexData(v6, n3, t2));
	vData.push_back(pntVertexData(v7, n3, t3));
	vData.push_back(pntVertexData(v3, n3, t4));

	// left face
	vData.push_back(pntVertexData(v5, n4, t1));
	vData.push_back(pntVertexData(v1, n4, t2));
	vData.push_back(pntVertexData(v4, n4, t3));
	vData.push_back(pntVertexData(v8, n4, t4));

	// back face
	vData.push_back(pntVertexData(v5, n2, t1));
	vData.push_back(pntVertexData(v6, n2, t2));
	vData.push_back(pntVertexData(v7, n2, t3));
	vData.push_back(pntVertexData(v8, n2, t4));

	// top
	vData.push_back(pntVertexData(v4, n6, t1));
	vData.push_back(pntVertexData(v3, n6, t2));
	vData.push_back(pntVertexData(v7, n6, t3));
	vData.push_back(pntVertexData(v8, n6, t4));

	// bot
	vData.push_back(pntVertexData(v6, n5, t1));
	vData.push_back(pntVertexData(v2, n5, t2));
	vData.push_back(pntVertexData(v1, n5, t3));
	vData.push_back(pntVertexData(v5, n5, t4));

	// Push back the indices for each face
	for (int i = 0; i < 6; i++) {
		indices.push_back(0 + 4 * i);
		indices.push_back(1 + 4 * i);
		indices.push_back(2 + 4 * i);
		indices.push_back(2 + 4 * i);
		indices.push_back(3 + 4 * i);
		indices.push_back(0 + 4 * i);
	}

	this->collisionShape = new btBoxShape(btVector3(halfWidth, halfHeight, halfDepth));

	this->subMeshes.push_back(MeshComponent::buildSubMesh(vData, indices, material));

} // end initialize
