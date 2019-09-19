#pragma once

#include "Mesh.h"

class FigureTwo : public Mesh
{
public:

	// Destructor
	virtual~FigureTwo();

	virtual void initialize(GLuint shaderProgram);

protected:

	GLuint VBO; // Buffer for positions and colors
};

