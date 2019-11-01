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
	std::vector<std::string> projViewMemberNames = { "modelMatrix", "normalModelMatrix", "viewingMatrix", "projectionMatrix"};

	std::vector<GLint> uniformOffsets = projViewBlock.setUniformBlockForShader(shaderProgram, transformBlockName, projViewMemberNames);

	// Save locations
	modelLocation = uniformOffsets[0];
	normalModelLocation = uniformOffsets[1];
	viewLocation = uniformOffsets[2];
	projectionLocation = uniformOffsets[3];

	uniformOffsets.clear();
	std::vector<std::string> worldEyeMemberNames = { "worldEyePosition" };

	uniformOffsets = worldEyeBlock.setUniformBlockForShader(shaderProgram, eyeBlockName, worldEyeMemberNames);

	// Save location
	eyePositionLocation = uniformOffsets[0];

} // end setUniformBlockForShader


void SharedProjectionAndViewing::setViewMatrix( glm::mat4 viewMatrix)
{
	if (projViewBlock.getSize() > 0  ) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer());

		SharedProjectionAndViewing::viewMatrix = viewMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, viewLocation, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	}

	if (worldEyeBlock.getSize() > 0 ) {

		// Bind the buffer.
		glBindBuffer(GL_UNIFORM_BUFFER, worldEyeBlock.getBuffer());

		glm::vec3 viewPoint = vec3(glm::inverse(viewMatrix)[3]);

		glBufferSubData(GL_UNIFORM_BUFFER, eyePositionLocation, sizeof(glm::vec3), glm::value_ptr(viewPoint));
	}

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

} // end setViewMatrix


// Accessor for the current viewing matrix
glm::mat4 SharedProjectionAndViewing::getViewMatrix()
{
	return viewMatrix;

} // end getViewMatrix


void SharedProjectionAndViewing::setProjectionMatrix( glm::mat4 projectionMatrix)
{

	if (projViewBlock.getSize()  > 0  ) {
	
		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer());

		SharedProjectionAndViewing::projectionMatrix = projectionMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, projectionLocation, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // end setProjectionMatrix


// Accessor for the current projection matrix
glm::mat4 SharedProjectionAndViewing::getProjectionMatrix()
{
	return projectionMatrix;

} // end getProjectionMatrix


void SharedProjectionAndViewing::setModelingMatrix(glm::mat4 modelingMatrix)
{
	if (projViewBlock.getSize() > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, projViewBlock.getBuffer() );

		SharedProjectionAndViewing::modelMatrix = modelingMatrix;

		glBufferSubData(GL_UNIFORM_BUFFER, modelLocation, sizeof(glm::mat4), glm::value_ptr(modelMatrix));

		glBufferSubData(GL_UNIFORM_BUFFER, normalModelLocation, sizeof(glm::mat3), 
			glm::value_ptr(mat3(glm::transpose(glm::inverse(modelMatrix)))));

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // end setModelingMatrix


// Accessor for the current modeling matrix
glm::mat4 SharedProjectionAndViewing::getModelingMatrix()
{
	return modelMatrix;

} // end getModelingMatrix

