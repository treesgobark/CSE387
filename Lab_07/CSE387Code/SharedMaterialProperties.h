#pragma once

#include "MathLibsConstsFuncs.h"
#include <vector>
//#include <iostream>

// Static helper classes to support uniform blocks
#include "SharedUniformBlock.h"

#define materialBlockBindingPoint 12
#define diffuseSamplerLocation 100
#define specularSamplerLocation 101
//#define normalMapSamplerLocation 102
//#define bumpMapSamplerLocation 103

struct Material
{
	Material()
		:diffuseTextureObject(0), specularTextureObject(0)//, normalMapObject(0)
	{
		setDefaultProperties();
	}

	void setDefaultProperties()
	{
		ambientMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		diffuseMat = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		specularMat = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		specularExpMat = 64.0f;
		emissiveMat = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		textureMode = 0; // NO_TEXTURE
		diffuseTextureEnabled = false;
		specularTextureEnabled = false;
		//normalMapEnabled = false;
		//bumpMapEnabled = false;
	}

	void setAmbientMat(glm::vec4 ambientMat)
	{
		this->ambientMat = ambientMat;
	}

	void setDiffuseMat(glm::vec4 diffuseMat)
	{
		this->diffuseMat = diffuseMat;
	}

	void setSpecularMat(glm::vec4 specularMat)
	{
		this->specularMat = specularMat;
	}

	void setSpecularExponentMat(float specularExpMat)
	{
		this->specularExpMat = specularExpMat;
	}

	void setEmissiveMat(glm::vec4 emissiveMat)
	{
		this->emissiveMat = emissiveMat;
	}

	void setAmbientAndDiffuseMat(glm::vec4 objectColor)
	{
		setAmbientMat(objectColor);
		setDiffuseMat(objectColor);

	}

	void setTextureMode(TextureMode textureMode)
	{
		switch (textureMode) {

		case NO_TEXTURE:
			this->textureMode = 0;
			break;
		case DECAL:
			this->textureMode = 1;
			break;
		case REPLACE_AMBIENT_DIFFUSE:
			this->textureMode = 2;
			break;
		default:
			std::cerr << "Illegal texture mode" << std::endl;
			break;
		}
	}

	void setDiffuseTexture(GLint textureObject)
	{
		this->diffuseTextureObject = textureObject;
		diffuseTextureEnabled = true;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);

	} // end setDiffuseTexture

	void setSpecularTexture(GLint textureObject)
	{
		this->specularTextureObject = textureObject;
		setTextureMode(REPLACE_AMBIENT_DIFFUSE);
		specularTextureEnabled = true;

	} // end setSpecularTexture

	//void setNormalMap(GLint textureObject)
	//{
	//	this->normalMapObject = textureObject;
	//	setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	normalMapEnabled = true;

	//} // end setNormalMap

	//void setBumpMap(GLint textureObject)
	//{
	//	this->bumpMapObject = textureObject;
	//	setTextureMode(REPLACE_AMBIENT_DIFFUSE);
	//	bumpMapEnabled = true;

	//} // end setNormalMap


	glm::vec4 ambientMat;

	glm::vec4 diffuseMat;

	glm::vec4 specularMat;

	float specularExpMat;

	glm::vec4 emissiveMat;

	int textureMode;

	GLuint diffuseTextureObject;
	bool diffuseTextureEnabled;

	GLuint specularTextureObject;
	bool specularTextureEnabled;

	//GLuint normalMapObject;
	//bool normalMapEnabled;

	//GLuint bumpMapObject;
	//bool bumpMapEnabled;
};


class SharedMaterialProperties
{
public:

	// Should be called for each shader program that includes the
	// Material*uniform block.
	static void setUniformBlockForShader(GLuint shaderProgram);

	// Call the set the Material*properties in the shader before 
	// rendering the object.
	static void setShaderMaterialProperties(Material*material);

	// Cleans Material*properties after rendering an object.
	static void cleanUpMaterial(Material*material);

protected:

	static GLuint ambientMatLocation; // Byte offset of the projection matrix

	static GLuint diffuseMatLocation; // Byte offset of the viewing matrix

	static GLuint specularMatLocation; // Byte offset of the modeling matrix

	static GLuint emmissiveMatLocation; // Byte offset of the modeling matrix to transform normal vectors

	static GLuint specularExpLocation;  // Byte offset of the eye position

	static GLuint diffuseTextureEnabledLocation;

	static GLuint specularTextureEnabledLocation;

	//static GLuint normalMapEnabledLocation;

	//static GLuint bumpMapEnabledLocation;

	static GLuint textureModeLoction;

	static SharedUniformBlock materialBlock;

	const static std::string materialBlockName;

};

