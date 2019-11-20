#include "SharedGeneralLighting.h"

#include <sstream> 

GeneralLight SharedGeneralLighting::lights[MAX_LIGHTS];

const std::string SharedGeneralLighting::generalLightBlockName = "LightBlock";

SharedUniformBlock SharedGeneralLighting::lightBlock(generalLightBlockBindingPoint);


void SharedGeneralLighting::setUniformBlockForShader(GLuint shaderProgram)
{
	std::vector <std::string > lightBlockMemberNames = buildUniformBlockNameList();

	std::vector<GLint> uniformOffsets = lightBlock.setUniformBlockForShader( shaderProgram, 
																		generalLightBlockName, 
																		lightBlockMemberNames);
	int offsetIndex = 0;

	for (int i = 0; i < MAX_LIGHTS; i++) {

		// Light color locations
		lights[i].ambientColorLoc = uniformOffsets[offsetIndex++];
		lights[i].diffuseColorLoc = uniformOffsets[offsetIndex++];
		lights[i].specularColorLoc = uniformOffsets[offsetIndex++];

		// Position and direction location
		lights[i].positionOrDirectionLoc = uniformOffsets[offsetIndex++];

		// Spotlight locations
		lights[i].spotDirectionLoc = uniformOffsets[offsetIndex++];
		lights[i].isSpotLoc = uniformOffsets[offsetIndex++];
		lights[i].spotCutoffCosLoc = uniformOffsets[offsetIndex++];
		lights[i].spotExponentLoc = uniformOffsets[offsetIndex++];

		//Attenuation factor locations
		lights[i].constantLoc = uniformOffsets[offsetIndex++];
		lights[i].linearLoc = uniformOffsets[offsetIndex++];
		lights[i].quadraticLoc = uniformOffsets[offsetIndex++];

		// Enabled location
		lights[i].enabledLoc = uniformOffsets[offsetIndex++];

		initilizeAttributes(i);
	}

} // end setUniformBlockForShader


std::vector<std::string> SharedGeneralLighting::buildUniformBlockNameList()
{
	static const int NUM_LIGHT_ATTRIBUTES = 12;

	// Get the indices of the uniform block variables in the shader
	static const std::string names[NUM_LIGHT_ATTRIBUTES]
		= { "ambientColor", "diffuseColor", "specularColor",
		"positionOrDirection", "spotDirection",
		"isSpot", "spotCutoffCos", "spotExponent",
		"constant", "linear", "quadratic", "enabled" };

	std::vector<std::string> uniformBlockNames;

	for (int lightNumber = 0; lightNumber < MAX_LIGHTS; lightNumber++) {

		// Get string representation of the int
		std::string numberStrg = std::to_string(lightNumber);

		std::string uniformNames[NUM_LIGHT_ATTRIBUTES];

		for (int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++) {

			uniformBlockNames.push_back("lights[" + numberStrg + "]." + names[i]);
		}

	}

	return uniformBlockNames;

}




void SharedGeneralLighting::initilizeAttributes(GLint lightNumber)
{
	setEnabled((lightSource)lightNumber, false);

	setAmbientColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setDiffuseColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setSpecularColor((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	setPositionOrDirection((lightSource)lightNumber, glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

	setIsSpot((lightSource)lightNumber, false);
	setSpotDirection((lightSource)lightNumber, glm::vec3(-1.0f, 1.0f, 0.0f));
	setSpotCutoffCos((lightSource)lightNumber, cos(glm::radians(180.0f)));
	setSpotExponent((lightSource)lightNumber, 50.0f);

	setConstantAttenuation((lightSource)lightNumber, 1.0f);
	setLinearAttenuation((lightSource)lightNumber, 0.0f);
	setQuadraticAttenuation((lightSource)lightNumber, 0.0f);

}


void SharedGeneralLighting::setEnabled(lightSource light, bool on)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].enabled = on;

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].enabledLoc, sizeof(bool), &lights[light].enabled);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAmbientColor(lightSource light, glm::vec4 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].ambientColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].ambientColorLoc, sizeof(glm::vec4), value_ptr(lights[light].ambientColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setDiffuseColor(lightSource light, glm::vec4 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].diffuseColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].diffuseColorLoc, sizeof(glm::vec4), value_ptr(lights[light].diffuseColor));

	// Unbind the buffer.
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpecularColor(lightSource light, glm::vec4 color4)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].specularColor = color4;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].specularColorLoc, sizeof(glm::vec4), value_ptr(lights[light].specularColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setPositionOrDirection(lightSource light, glm::vec4 positOrDirect)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].positionOrDirection = positOrDirect;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].positionOrDirectionLoc, sizeof(glm::vec4), value_ptr(lights[light].positionOrDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAttenuationFactors(lightSource light, glm::vec3 factors)
{
	setConstantAttenuation(light, factors.x);
	setLinearAttenuation(light, factors.y);
	setQuadraticAttenuation(light, factors.z);

}

void SharedGeneralLighting::setConstantAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].constant = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].constantLoc, sizeof(float), &lights[light].constant);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setLinearAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].linear = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].linearLoc, sizeof(float), &lights[light].linear);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setQuadraticAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].quadratic = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].quadraticLoc, sizeof(float), &lights[light].quadratic);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setIsSpot(lightSource light, bool spotOn)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].isSpot = spotOn;

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].isSpotLoc, sizeof(bool), &lights[light].isSpot);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotDirection(lightSource light, glm::vec3 spotDirect)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].spotDirection = glm::normalize(spotDirect);
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotDirectionLoc, sizeof(glm::vec3), value_ptr(lights[light].spotDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotCutoffCos(lightSource light, float cutoffCos)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].spotCutoffCos = cutoffCos;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotCutoffCosLoc, sizeof(float), &lights[light].spotCutoffCos);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotExponent(lightSource light, float spotEx)
{
	glBindBuffer(GL_UNIFORM_BUFFER, lightBlock.getBuffer());

	lights[light].spotExponent = spotEx;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotExponentLoc, sizeof(float), &lights[light].spotExponent);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}