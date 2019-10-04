#include "SharedProjectionAndViewing.h"

GLuint SharedProjectionAndViewing::projectionLocation; // Byte offset of the projection matrix
glm::mat4 SharedProjectionAndViewing::projectionMatrix; // Current projection matrix that is held in the buffer

GLuint SharedProjectionAndViewing::viewLocation; // Byte offset of the viewing matrix
glm::mat4 SharedProjectionAndViewing::viewMatrix; // Current viewing matrix that is held in the buffer

GLuint SharedProjectionAndViewing::modelLocation; // Byte offset of the modeling matrix
glm::mat4 SharedProjectionAndViewing::modelMatrix; // Current modeling matrix that is held in the buffer

GLuint SharedProjectionAndViewing::normalModelLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedProjectionAndViewing::eyePositionLocation;  // Byte offset of the eye position

SharedUniformBlock SharedProjectionAndViewing::projViewBlock(projectionViewBlockBindingPoint);
SharedUniformBlock SharedProjectionAndViewing::worldEyeBlock(worldEyeBlockBindingPoint);

const std::string SharedProjectionAndViewing::transformBlockName = "transformBlock";

const std::string SharedProjectionAndViewing::eyeBlockName = "worldEyeBlock";


void SharedProjectionAndViewing::setUniformBlockForShader(GLuint shaderProgram)
{
	// Build vectors of strings containing the names of the variable in each of the uniform blocks
	// TODO
	
	// Get the locations in the two different uniform blocks
	// TODO
	
	// Save the locations in class variables so they can be used to set values in the buffer
	// TODO

} // end setUniformBlockForShader

// Complete the implementation of this class
// TODO
