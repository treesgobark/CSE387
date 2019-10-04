#pragma once
#include <string>

#include "Mesh.h"

// Includes for model loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

/**
 * @class	ModelMesh
 *
 * @brief	Class for loading vertex data and material properties including textures. Loaded 
 * 			properties are stored in SubMesh structs that rendered by the Mesh super class.
 */
class ModelMesh : public Mesh
{
public:

	/**
	 * @fn	ModelMesh::ModelMesh(string filePathAndName);
	 *
	 * @brief	Constructor
	 *
	 * @param	filePathAndName	Relative path and file name for the model to be loaded.
	 */
	ModelMesh(string filePathAndName);


	/**
	 * @fn	virtual void ModelMesh::initialize(GLuint shaderProgram) override;
	 *
	 * @brief	Reads in the model using Assimp and builds necessary sub-meshes.
	 *
	 * @param	shaderProgram	The shader program.
	 */
	virtual void initialize(GLuint shaderProgram) override;

protected:

	/** @brief	Relative path and file name for the model */
	string filePathAndName;

	/**
	 * @fn	std::string ModelMesh::getDirectoryPath(std::string sFilePath);
	 *
	 * @brief	Separates the file name from the relative path and returns the path
	 *
	 * @param	sFilePath	Full pathname of the file.
	 *
	 * @returns	The directory path.
	 */
	std::string getDirectoryPath(std::string sFilePath);

	/**
	 * @fn	void ModelMesh::readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices, btConvexHullShape& hull);
	 *
	 * @brief	Reads vertex data and places it in data structures and variables that are passed
	 * 			by reference.
	 *
	 * @param [out]	mesh	  	If non-null, the mesh.
	 * @param [out]	vertexData	Information describing the vertex.
	 * @param [out]	indices   	The indices.
	 * @param [out]	hull	  	The hull.
	 */
	void readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices, btConvexHullShape& hull);

	/**
	 * @fn	Material* ModelMesh::readInMaterialProperties( aiMaterial* assimpMaterial, std::string filename);
	 *
	 * @brief	Copies in material properties from an AiMaterial struct to a Material struct.
	 *
	 * @param [in]	assimpMaterial	If non-null, the assimp material.
	 * @param 		filename	  	Filename of the file.
	 *
	 * @returns	Default Material struct if it fails, else the in material properties.
	 */
	Material* readInMaterialProperties(const aiMaterial* assimpMaterial, std::string filename);

};

