#pragma once

#include "MathLibsConstsFuncs.h"
#include <vector>

class SharedUniformBlock
{

public:

	SharedUniformBlock(GLint blockBindingPoint) : blockBindingPoint(blockBindingPoint) {}

	~SharedUniformBlock() { glDeleteBuffers(1, &blockBuffer); }

	std::vector<GLint>  setUniformBlockForShader(GLuint shaderProgram, std::string blockName, std::vector<std::string> blockMembers);

	GLint getSize() { return blockSize; }

	GLuint getBuffer(){ return blockBuffer; }

private:

	// Determines the size in bytes of the block and binds it to the binding point. 
	void determineBlockSizeSetBindingPoint(GLuint shaderProgram, std::string blockName);
	
	// Creates the buffer and binds it to the binding point.
	void allocateBuffer(GLuint shaderProgram);

	// Finds the byte offsets of the variables in the two uniform  blocks.
	void findOffsets(GLuint shaderProgram, std::vector<std::string> blockMembers);
	// Identifier for the uniform block.
	GLuint blockIndex;

	// Size in bytes of both the buffer and the uniform block in all the the shaders.
	GLint blockSize;

	// Binding point to which the blocks and buffer will be bound
	GLint blockBindingPoint;

	// Identifier for the buffer. There would only be one buffer to feed several uniform blocks.
	GLuint blockBuffer;

	// Indicates whether or not the buffer for the blocks has been setup
	bool blockSizeAndOffetsSet; 

	// Holds byte offsets for the members of the uniform block
	std::vector<GLint> offsets;

}; // end SharedUniformBlock


bool checkBlockLocationFound(const GLchar* locationName, GLuint indice);


