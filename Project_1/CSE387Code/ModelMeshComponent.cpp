#include "ModelMeshComponent.h"


ModelMeshComponent::ModelMeshComponent (string filePathAndName, GLuint shaderProgram, int updateOrder)
	: MeshComponent(shaderProgram, updateOrder), filePathAndName(filePathAndName)
{
}

void ModelMeshComponent::initialize()
{
	// Model loading
	std::vector<pntVertexData> vData;
	std::vector<unsigned int> indices;

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// Load the scene/model and associated meshes into a aiScene object
	// See http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html
	// for more details. Second argument specifies configuration that is optimized for 
	// real-time rendering.
	const aiScene* scene = importer.ReadFile(filePathAndName, aiProcessPreset_TargetRealtime_Quality);//aiProcess_CalcTangentSpace);

	// Check if the scene/model loaded correctly
	if (!scene) {
		std::cerr << "ERROR: Unable to load " << filePathAndName <<
			std::endl << "\t" << importer.GetErrorString() << std::endl;
	}
	else {
		std::cout << "Loading Model: " << filePathAndName << std::endl;
	}

	/*
	This is a concave shape made out of convex sub parts, called child shapes. Each
	child shape has its own local offset transform, relative to the btCompoundShape. It is a good idea to
	approximate concave shapes using a collection of convex hulls, and store them in a
	btCompoundShape.
	*/
	// Create compound shape to hold the shapes of the individual meshes
	btCompoundShape* modelCompondShape = new btCompoundShape();

	// Iterate through each mesh
	for (size_t i = 0; i < scene->mNumMeshes; i++) {

		// Get the vertex mesh 
		aiMesh* mesh = scene->mMeshes[i];

		// Create a collision shape for the mesh
		btConvexHullShape* meshCollisionShape = new btConvexHullShape();

		// Read in the vertex data associated with the model
		readVertexData(mesh, vData, indices, *meshCollisionShape);

		// Get the Material*for the mesh
		aiMaterial* meshMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

		// Create an empty material for the sub mesh
		Material* material = NULL;

		// Read in the Material*properties for this mesh
		if (mesh->mMaterialIndex >= 0) {

			material = readInMaterialProperties(meshMaterial, filePathAndName);

		}
		else {
			material = new Material(); // Use the default material settings
		}

		this->subMeshes.push_back(buildSubMesh(vData, indices, material));

		// Add the mesh collision shape for collision detection
		// Do NOT use the default btTransform constructor for this! It  
		// makes a zero matrix and everything disappears. No problem for collision spheres! 
		modelCompondShape->addChildShape(btTransform(btQuaternion(0, 0, 0)), meshCollisionShape);

		vData.clear();
		indices.clear();
	}

	// Save the compond collision shape for this model
	this->collisionShape = modelCompondShape;

} // end initialize


void ModelMeshComponent::readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices, btConvexHullShape& hull)
{
	// Read in vertex positions, normals, and texture coordinates. See 
	// http://www.assimp.org/lib_html/structai_MeshComponent.html for more details
	if (mesh->HasPositions()) {
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			// Read in vertex position data
			glm::vec3 tempPosition;
			tempPosition.x = mesh->mVertices[i].x;
			tempPosition.y = mesh->mVertices[i].y;
			tempPosition.z = mesh->mVertices[i].z;

			// Add the vertex for the collision shape
			hull.addPoint(btVector3(tempPosition.x, tempPosition.y, tempPosition.z), true);

			// Read in vertex normal vectors
			glm::vec3 tempNormal;
			tempNormal.x = mesh->mNormals[i].x;
			tempNormal.y = mesh->mNormals[i].y;
			tempNormal.z = mesh->mNormals[i].z;

			// Read in texture coordinates
			glm::vec2 tempCoord(0.0f, 0.0f);
			// Should always be true since coordinates are auto generated if not present
			if (mesh->HasTextureCoords(0)) {
				tempCoord.x = mesh->mTextureCoords[0][i].x;
				tempCoord.y = mesh->mTextureCoords[0][i].y;
			}

			// Push all data for this vertex into the data vector in preparation for buffering
			vertexData.push_back(pntVertexData(tempPosition, tempNormal, tempCoord));
		}
	}

	// Read in the indices that describe faces in preparation for buffering
	if (mesh->HasFaces()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	}

}

std::string ModelMeshComponent::getDirectoryPath(std::string sFilePath)
{
	// Get directory path
	std::string sDirectory = "";

	for (int i = (int)sFilePath.size() - 1; i >= 0; i--) {

		if (sFilePath[i] == '\\' || sFilePath[i] == '/') {
			sDirectory = sFilePath.substr(0, i + 1);
			break;
		}
	}
	return sDirectory;
}

Material* ModelMeshComponent::readInMaterialProperties( const aiMaterial* assimpMaterial, std::string filename)
{
	// This is dynamically allocated here and will be deleted in the 
	// destructor when the subMesh that is is assigned to is deleted.
	Material* meshMaterial = new Material();

	// Read in the name of the material
	aiString name;
	assimpMaterial->Get(AI_MATKEY_NAME, name);
	std::cout << "Loading " << name.C_Str() << " material:" << std::endl;

	// Temporary assimp color to read into
	aiColor3D matColor(0.0f, 0.0f, 0.0f);

	// Process materials (See http://www.assimp.org/lib_html/materials.html for details)

	// Query for ambient color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, matColor) == AI_SUCCESS) {

		meshMaterial->setAmbientMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
	}
	// Query for diffuse color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, matColor) == AI_SUCCESS) {

		meshMaterial->setDiffuseMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
	}
	// Query for specular color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, matColor) == AI_SUCCESS) {

		meshMaterial->setSpecularMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
	}
	// Query for emissive color
	if (assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, matColor) == AI_SUCCESS) {

		meshMaterial->setEmissiveMat(glm::vec4(matColor[0], matColor[1], matColor[2], 1.0));
	}

	// Temporary to hold the path to a texture
	aiString path;

	// Load diffuse, specular, and normal maps
	if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();

			meshMaterial->setDiffuseTexture(Texture::GetTexture(relativeFilePath)->getTextureObject());
		}
	}
	if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();

			meshMaterial->setSpecularTexture(Texture::GetTexture(relativeFilePath)->getTextureObject());
		}
	}
	//if (assimpMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {

	//	if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

	//		std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();

	//		meshMaterial->setNormalMap(Texture::GetTexture(relativeFilePath)->getTextureObject());
	//	}
	//}

	meshMaterial->setTextureMode(REPLACE_AMBIENT_DIFFUSE);

	return meshMaterial;

} // end readInMaterialProperties
