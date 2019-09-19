#include "FigureTwo.h"


FigureTwo::~FigureTwo()
{
	glDeleteBuffers(1, &VBO);

} // end destructor


void FigureTwo::initialize(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;
	glUseProgram(this->shaderProgram);

	//	cout << "fig one initialize" << endl;

	const vec4 posAndCol[72] = { vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),
							     vec4(-0.25,  0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),
							     vec4( 0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),
							     vec4(-0.25,  0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),
							     vec4( 0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),
							     vec4( 0.25,  0.25, -0.25, 1.0), vec4(0.9, 0.1, 0.1, 1.0),

								 vec4(0.25,  0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
								 vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
								 vec4(0.25,  0.25,  0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
								 vec4(0.25, -0.25, -0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
								 vec4(0.25,  0.25,  0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
								 vec4(0.25, -0.25,  0.25, 1.0), vec4(0.9, 0.5, 0.1, 1.0),
	
								 vec4(-0.25, -0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),
								 vec4( 0.25, -0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),
								 vec4( 0.25, -0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),
								 vec4( 0.25,  0.25,  0.25, 1.0), vec4(0.9, 0.9, 0.1, 1.0),

								 vec4(-0.25,  0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),
								 vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),
								 vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),
								 vec4(-0.25, -0.25,  0.25, 1.0), vec4(0.1, 0.9, 0.1, 1.0),

								 vec4(-0.25,  0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),
								 vec4( 0.25,  0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),
								 vec4(-0.25,  0.25,  0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),
								 vec4( 0.25,  0.25, -0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),
								 vec4( 0.25,  0.25,  0.25, 1.0), vec4(0.1, 0.1, 0.9, 1.0),

								 vec4(-0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0),
								 vec4(-0.25, -0.25,  0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0),
								 vec4( 0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0),
								 vec4(-0.25, -0.25,  0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0),
								 vec4( 0.25, -0.25, -0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0),
								 vec4( 0.25, -0.25,  0.25, 1.0), vec4(0.5, 0.1, 0.9, 1.0), };

	// Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Load positions and colors into the same buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(posAndCol), posAndCol, GL_STATIC_DRAW);

	// Specify the location and data format of an array of vertex positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 16 + 16, 0);
	glEnableVertexAttribArray(0);

	// Specify the location and data format of an array of vertex colors
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 16 + 16, (const void*)16);
	glEnableVertexAttribArray(1);

	renderMode = ORDERED;

	// Set the number of vertices to be processed in the vertex fetch
	count = 36;

	modelLocation = 2;

}