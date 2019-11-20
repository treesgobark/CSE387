#pragma once
#include "MeshComponent.h"
class SphereMeshComponent : public MeshComponent
{
public:

	SphereMeshComponent(GLuint shaderProgram, Material * material, GLfloat radius = 2.0f, int updateOrder = 100, GLint stacks = 8, GLint slices = 16);

	virtual void initialize() override;

protected:

	void initializeTop();
	void initializeBody();
	void initializeBottom();

	GLint stacks;
	GLint slices;
	GLfloat radius;

	std::vector<GLfloat> stackAngles;
	std::vector<GLfloat> sliceAngles;

	std::vector<pntVertexData> v;
	std::vector<unsigned int> indices;

	unsigned int indiceCounter = 0;

	Material * material;



};

