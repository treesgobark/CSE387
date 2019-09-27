#pragma once
#include <string>

#include "Mesh.h"

// Includes for model loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "SharedMaterialProperties.h"


struct SubMesh {

	GLuint vao;

	GLuint vertexBuffer;

	GLuint indexBuffer;

	Material * modelMaterial;

	GLuint indexCount;
};



struct pntVertexData
{
	glm::vec3 m_pos = ZERO_V3;
	glm::vec3 m_normal = ZERO_V3;
	glm::vec2 m_textCoord = ZERO_V2;

	pntVertexData() {}

	pntVertexData(glm::vec3 pos, glm::vec3 normal, glm::vec2 textCoord)
	{
		m_pos = pos;
		m_normal = normal;
		m_textCoord = textCoord;
	}
};


class ModelMesh : public Mesh
{
public:

	// Default constructor for early labs
	ModelMesh(string filePathAndName);

	// Destructor
	virtual~ModelMesh();

	virtual void initialize(GLuint shaderProgram) override;

	virtual void draw() override;

protected:

	string filePathAndName;

	GLenum primitiveMode;

	Texture texture;

	std::string getDirectoryPath(std::string sFilePath);
	
	void readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices/*, btConvexHullShape& hull*/);

	Material* readInMaterialProperties(/*Renderer* renderer, */aiMaterial* assimpMaterial, std::string filename);

	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material);

	std::vector<SubMesh> subMeshes;

};

