#include "SphereMeshComponent.h"

// Get the Spherical texture mapping coordinates based on slice and 
// stack angles
glm::vec2 getSphericalTextCoords(float sliceAngle, float stackAngle)
{
	float s = sliceAngle / (2.0f * PI);
	float t = (stackAngle + PI / 2) / PI;

	return glm::vec2(s, t);
}


/**
* Generates Cartesion xyz coordinates on the surface of a sphere of a specified
* radius from polar coordinates. Assumes the "poles" of the sphere on on the Y
* axis. Stack angles are relative the Y axis. Slice angles
* are around the Y axis in plane that is perpendicular to it.
* @param radius of the sphere on which the Cartesion coordinates are being generated
* @param sliceAngle - angle in the XZ plane
* @param stackAngle - angle relative to the Y axis.
* @returns homogenous 4 dimensional vector containing the Cartesion coordinates
* for the specified slice and stack angles.
*/
vec3 sphericalToCartesion( float sliceAngle, float stackAngle, float radius)
{
	vec3 v(glm::cos(stackAngle) * glm::sin(sliceAngle) * radius,
		glm::sin(stackAngle) * radius,
		glm::cos(stackAngle) * cos(sliceAngle) * radius);

	return v;

} // end sphericalToCartesion


SphereMeshComponent::SphereMeshComponent(GLuint shaderProgram, Material * material, 
										 GLfloat radius, int updateOrder, GLint stacks, GLint slices)
	: material(material),radius(radius), stacks(stacks), slices(slices), MeshComponent (shaderProgram, updateOrder)
{
	GLfloat stackInc = (PI) / (stacks);
	GLfloat sliceInc = (2 * PI) / (slices);

	GLfloat angle = -PI / 2 + stackInc;
	stackAngles.push_back(angle);

	for (int i = 1; i < stacks - 1; i++) {

		angle += stackInc;
		stackAngles.push_back(angle);
	}

	angle = 0;
	sliceAngles.push_back(angle);

	for (int i = 0; i < slices; i++) {

		angle += sliceInc;
		sliceAngles.push_back(angle);
	}

}


void SphereMeshComponent::initialize()
{
	initializeTop();
	initializeBody();
	initializeBottom();

	this->subMeshes.push_back(MeshComponent::buildSubMesh(v, indices, material));

	// Clear the working vectors used to make the vertex data
	v.clear();
	stackAngles.clear();
	sliceAngles.clear();

	this->collisionShape = new btSphereShape(radius);
}

void SphereMeshComponent::initializeTop()
{
	vec3 vertTop = vec3(0, radius, 0);
	vec3 normTop = vec3(glm::normalize(vertTop));

	vec3 vert = sphericalToCartesion(sliceAngles[0], stackAngles[stackAngles.size() - 1], radius);
	vec3 norm = vec3(glm::normalize(vert));
	vec2 tex = getSphericalTextCoords(sliceAngles[0], stackAngles[stackAngles.size() - 1]);

	for (int sliceIndex = 0; sliceIndex < slices; sliceIndex++) {

		vec2 texTop = getSphericalTextCoords((sliceAngles[sliceIndex] + sliceAngles[sliceIndex + 1]) / 2, PI / 2);

		vec3 vert2 = vert;
		vec3 norm2 = norm;
		vec2 tex2 = tex;

		vert = sphericalToCartesion(sliceAngles[sliceIndex + 1], stackAngles[stackAngles.size() - 1], radius);
		norm = vec3(glm::normalize(vert));
		tex = getSphericalTextCoords(sliceAngles[sliceIndex + 1], stackAngles[stackAngles.size() - 1]);

		v.push_back(pntVertexData(vertTop, normTop, texTop));
		indices.push_back(indiceCounter++);
		v.push_back(pntVertexData(vert2, norm2, tex2));
		indices.push_back(indiceCounter++);
		v.push_back(pntVertexData(vert, norm, tex));
		indices.push_back(indiceCounter++);
	}

} // end initializeTop

void SphereMeshComponent::initializeBody()
{
	for (unsigned int stackIndex = 0; stackIndex < stackAngles.size() - 1; stackIndex++) {

		for (unsigned int sliceIndex = 0; sliceIndex < sliceAngles.size() - 1; sliceIndex++) {

			vec3 vert0 = sphericalToCartesion(sliceAngles[sliceIndex], stackAngles[stackIndex + 1], radius);
			vec2 text0 = getSphericalTextCoords(sliceAngles[sliceIndex], stackAngles[stackIndex + 1]);
			vec3 vert1 = sphericalToCartesion(sliceAngles[sliceIndex], stackAngles[stackIndex], radius);
			vec2 text1 = getSphericalTextCoords(sliceAngles[sliceIndex], stackAngles[stackIndex]);
			vec3 vert2 = sphericalToCartesion(sliceAngles[sliceIndex + 1], stackAngles[stackIndex], radius);
			vec2 text2 = getSphericalTextCoords(sliceAngles[sliceIndex + 1], stackAngles[stackIndex]);
			vec3 vert3 = sphericalToCartesion(sliceAngles[sliceIndex + 1], stackAngles[stackIndex + 1], radius);
			vec2 text3 = getSphericalTextCoords(sliceAngles[sliceIndex + 1], stackAngles[stackIndex + 1]);

			v.push_back(pntVertexData(vert0, vec3(glm::normalize(vert0)), text0));
			indices.push_back(indiceCounter++);
			v.push_back(pntVertexData(vert1, vec3(glm::normalize(vert1)), text1));
			indices.push_back(indiceCounter++);
			v.push_back(pntVertexData(vert2, vec3(glm::normalize(vert2)), text2));
			indices.push_back(indiceCounter++);

			v.push_back(pntVertexData(vert0, vec3(glm::normalize(vert0)), text0));
			indices.push_back(indiceCounter++);
			v.push_back(pntVertexData(vert2, vec3(glm::normalize(vert2)), text2));
			indices.push_back(indiceCounter++);
			v.push_back(pntVertexData(vert3, vec3(glm::normalize(vert3)), text3));
			indices.push_back(indiceCounter++);
		}
	}

} // end initializeBody


void SphereMeshComponent::initializeBottom()
{
	vec3 vertBottom = vec3(0, -radius, 0);
	vec3 normBottom = vec3(glm::normalize(vertBottom));

	vec3 vert = sphericalToCartesion(sliceAngles[slices], stackAngles[0], radius);
	vec3 norm = vec3(glm::normalize(vert));
	vec2 tex = getSphericalTextCoords(sliceAngles[slices], stackAngles[0]);

	for (int sliceIndex = slices; sliceIndex > 0; sliceIndex--) {

		vec2 texBottom = getSphericalTextCoords((sliceAngles[sliceIndex] + sliceAngles[sliceIndex - 1]) / 2, -PI / 2);

		vec3 vert2 = vert;
		vec3 norm2 = norm;
		vec2 tex2 = tex;

		vert = sphericalToCartesion(sliceAngles[sliceIndex - 1], stackAngles[0], radius);
		norm = vec3(glm::normalize(vert));
		tex = getSphericalTextCoords(sliceAngles[sliceIndex - 1], stackAngles[0]);

		v.push_back(pntVertexData(vertBottom, normBottom, texBottom));
		indices.push_back(indiceCounter++);
		v.push_back(pntVertexData(vert2, norm2, tex2));
		indices.push_back(indiceCounter++);
		v.push_back(pntVertexData(vert, norm, tex));
		indices.push_back(indiceCounter++);
	}

} // end initializeBottom


