#pragma once

#include "Mesh.h"

class FigureOne : 	public Mesh
{
	public:

	// Destructor
	virtual~FigureOne();

	virtual void initialize(GLuint shaderProgram);

	protected:

	GLuint VBO; // Buffer for positions and colors
};

