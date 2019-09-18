#include "Mesh.h"



 
Mesh::~Mesh()
{ 
	glDeleteVertexArrays (1, &VAO);

} // end destructor


// Preform drawing operations. 
void Mesh::draw()
{
	// Bind vertex array object
	glBindVertexArray(VAO);

	// Use the shader program for this object
	glUseProgram(shaderProgram);

	// Set Modeling transformation
	GLenum primitiveMode = GL_TRIANGLES;

	if (renderMode == ORDERED) {

		// Fetch input data for pipeline	
		glDrawArrays(primitiveMode, 0, count);

	}
	else { // renderMode == INDEXED

		// Fetch input data for pipeline	
		glDrawElements(primitiveMode, count, GL_UNSIGNED_INT, 0);
	}

} // end draw











