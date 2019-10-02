#include "SharedMaterialProperties.h"

GLuint SharedMaterialProperties::ambientMatLocation; // Byte offset of the projection matrix

GLuint SharedMaterialProperties::diffuseMatLocation; // Byte offset of the viewing matrix

GLuint SharedMaterialProperties::specularMatLocation; // Byte offset of the modeling matrix

GLuint SharedMaterialProperties::emmissiveMatLocation; // Byte offset of the modeling matrix to transform normal vectors

GLuint SharedMaterialProperties::specularExpLocation;  // Byte offset of the eye position

GLuint SharedMaterialProperties::diffuseTextureEnabledLocation;

GLuint SharedMaterialProperties::specularTextureEnabledLocation;

GLuint SharedMaterialProperties::normalMapEnabledLocation;

GLuint SharedMaterialProperties::bumpMapEnabledLocation;

GLuint SharedMaterialProperties::textureModeLoction;

SharedUniformBlock SharedMaterialProperties::materialBlock(materialBlockBindingPoint);

const std::string SharedMaterialProperties::materialBlockName = "MaterialBlock";


void SharedMaterialProperties::setUniformBlockForShader(GLuint shaderProgram)
{
	std::vector<std::string> materialMemberNames = { "object.ambientMat", "object.diffuseMat", "object.specularMat",
										   "object.emmissiveMat", "object.specularExp", 
										   "object.diffuseTextureEnabled" , "object.specularTextureEnabled" ,
											"object.normalMapEnabled", "object.bumpMapEnabled","object.textureMode" };

	std::vector<GLint> uniformOffsets = materialBlock.setUniformBlockForShader(shaderProgram, materialBlockName, materialMemberNames);

	// Save locations
	ambientMatLocation = uniformOffsets[0];
	diffuseMatLocation = uniformOffsets[1];
	specularMatLocation = uniformOffsets[2];
	emmissiveMatLocation = uniformOffsets[3];
	specularExpLocation = uniformOffsets[4];
	diffuseTextureEnabledLocation = uniformOffsets[5];
	specularTextureEnabledLocation = uniformOffsets[6];
	normalMapEnabledLocation = uniformOffsets[7];
	bumpMapEnabledLocation = uniformOffsets[8];
	textureModeLoction = uniformOffsets[9];

} // end setUniformBlockForShader


void SharedMaterialProperties::setShaderMaterialProperties(Material* material)
{
	if (materialBlock.getSize() > 0) {

		// Bind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, materialBlock.getBuffer());

		// Set the Material*properties in the shader.
		glBufferSubData(GL_UNIFORM_BUFFER, ambientMatLocation, sizeof(glm::vec4), glm::value_ptr(material->ambientMat));
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseMatLocation, sizeof(glm::vec4), glm::value_ptr(material->diffuseMat));
		glBufferSubData(GL_UNIFORM_BUFFER, specularMatLocation, sizeof(glm::vec4), glm::value_ptr(material->specularMat));
		glBufferSubData(GL_UNIFORM_BUFFER, emmissiveMatLocation, sizeof(glm::vec4), glm::value_ptr(material->emissiveMat));
		glBufferSubData(GL_UNIFORM_BUFFER, specularExpLocation, sizeof(float), &material->specularExpMat);
		glBufferSubData(GL_UNIFORM_BUFFER, diffuseTextureEnabledLocation, sizeof(bool), &material->diffuseTextureEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, specularTextureEnabledLocation, sizeof(bool), &material->specularTextureEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, normalMapEnabledLocation, sizeof(bool), &material->normalMapEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, bumpMapEnabledLocation, sizeof(bool), &material->bumpMapEnabled);
		glBufferSubData(GL_UNIFORM_BUFFER, textureModeLoction, sizeof(int), &material->textureMode);

		// Unbind the buffer. 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Activate and set texture units.
		if (material->diffuseTextureEnabled == true) {
			glUniform1i(diffuseSamplerLocation, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->diffuseTextureObject);
		}
		if (material->specularTextureEnabled == true) {
			glUniform1i(specularSamplerLocation, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material->specularTextureObject);
		}
		if (material->normalMapEnabled == true) {
			glUniform1i(normalMapSamplerLocation, 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, material->normalMapObject);
		}
		if (material->bumpMapEnabled == true) {
			glUniform1i(bumpMapSamplerLocation, 3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, material->bumpMapObject);
		}
	}

} // end setShaderMaterialProperties


void SharedMaterialProperties::cleanUpMaterial(Material*material)
{
	if (material->diffuseTextureEnabled == true) {
		glUniform1i(diffuseSamplerLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (material->specularTextureEnabled == true) {
		glUniform1i(specularSamplerLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (material->normalMapEnabled == true) {
		glUniform1i(normalMapSamplerLocation, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (material->bumpMapEnabled == true) {
		glUniform1i(bumpMapSamplerLocation, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
