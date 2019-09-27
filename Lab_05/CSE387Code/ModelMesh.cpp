#include "ModelMesh.h"

#include "Texture.h"

// Default constructor for early labs
ModelMesh::ModelMesh (string filePathAndName )
	: filePathAndName(filePathAndName)
{
}


ModelMesh::~ModelMesh()
{
	for (auto& subMesh : subMeshes) {

		glDeleteVertexArrays(1, &subMesh.vao);

		glDeleteBuffers(1, &subMesh.vertexBuffer);

		glDeleteBuffers(1, &subMesh.indexBuffer);
	}

} // end destructor


void ModelMesh::initialize(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;

	// Model loading
	std::vector<pntVertexData> vData;
	std::vector<unsigned int> indices;

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// Load the scene/model and associated meshes into a aiScene object
	// See http://www.assimp.org/lib_html/class_assimp_1_1_importer.html#afa338a135a56956bd5deb7d238498dde 
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
	//btCompoundShape* modelCompondShape = new btCompoundShape();

	// Iterate through each mesh
	for (size_t i = 0; i < scene->mNumMeshes; i++) {

		// Get the vertex mesh 
		aiMesh* mesh = scene->mMeshes[i];

		// Create a collision shape for the mesh
		//btConvexHullShape* meshCollisionShape = new btConvexHullShape();

		// Read in the vertex data associated with the model
		readVertexData(mesh, vData, indices/*, *meshCollisionShape*/);

		// Get the Material*for the mesh
		aiMaterial* meshMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

		// Create an empty material for the sub mesh
		Material* material = new Material();

		// Read in the Material*properties for this mesh
		if (mesh->mMaterialIndex >= 0) {

			material = readInMaterialProperties(/*renderer,*/ meshMaterial, filePathAndName);
		}

		buildSubMesh(vData, indices, material);

		// Add the mesh collision shape for collision detection
		// Do NOT use the default btTransform constructor for this! It apparently makes a zero matrix and everything disappears.
		// No problem for collision spheres! 
		//modelCompondShape->addChildShape(btTransform(btQuaternion(0, 0, 0)), meshCollisionShape);

		vData.clear();
		indices.clear();
	}

	this->primitiveMode = GL_TRIANGLES;

	// Save the compond collision shape for this model
	//this->collisionShape = modelCompondShape;


}



SubMesh ModelMesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material * material)
{
	SubMesh subMesh;
	
	// Generate and bind the vertex array object
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	// Gnerate, bind, and buffer the Vertex Array Buffer
	count = static_cast<unsigned int>(vertexData.size());

	glGenBuffers(1, &subMesh.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, subMesh.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(pntVertexData), &vertexData[0], GL_STATIC_DRAW);

	// Specify the location and data format of the positions, normals, and texture coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	// Generate, bind, and buffer the Index Array Buffer
	subMesh.indexCount = static_cast<unsigned int>(indices.size());
	glGenBuffers(1, &subMesh.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	return subMesh;
}



void ModelMesh::readVertexData(aiMesh* mesh, std::vector<pntVertexData>& vertexData, std::vector<unsigned int>& indices/*, btConvexHullShape& hull*/)
{
	// Read in vertex positions, normals, and texture coordinates. See 
	// http://www.assimp.org/lib_html/structai_mesh.html for more details
	if (mesh->HasPositions()) {
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

			// Read in vertex position data
			glm::vec3 tempPosition;
			tempPosition.x = mesh->mVertices[i].x;
			tempPosition.y = mesh->mVertices[i].y;
			tempPosition.z = mesh->mVertices[i].z;

			// Add the vertex for the collision shape
			//hull.addPoint(btVector3(tempPosition.x, tempPosition.y, tempPosition.z), true);

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

std::string ModelMesh::getDirectoryPath(std::string sFilePath)
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

Material* ModelMesh::readInMaterialProperties(/*Renderer* renderer, */aiMaterial* assimpMaterial, std::string filename)
{
	Material* meshMaterial = new Material();

	// Read in the name of the material
	aiString name;
	assimpMaterial->Get(AI_MATKEY_NAME, name);
	std::cout << "Loading " << name.C_Str() << " material:" << std::endl;

	// Default Material*Color
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

	// Path to the texture
	aiString path;

	// Load diffuse, specular, and normal maps
	if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			std::cout << "Loading texture: " << relativeFilePath << std::endl;

			Texture diffuseMap;
			
			diffuseMap.load(relativeFilePath);

			meshMaterial->setDiffuseTexture(diffuseMap.getTextureObject());
		}
	}
	if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			std::cout << "Loading texture: " << relativeFilePath << std::endl;

			Texture specMap;
			
			specMap.load(relativeFilePath);

			meshMaterial->setSpecularTexture(specMap.getTextureObject());
		}
	}
	if (assimpMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {

		if (AI_SUCCESS == assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &path, nullptr, nullptr, nullptr, nullptr, nullptr)) {

			std::string relativeFilePath = getDirectoryPath(filename) + path.C_Str();
			std::cout << "Loading texture: " << relativeFilePath << std::endl;

			Texture normalMap;

			normalMap.load(relativeFilePath);

			meshMaterial->setNormalMap(normalMap.getTextureObject());
		}
	}

	return meshMaterial;

} // end readInMaterialProperties

// Render the object and all of its children
void ModelMesh::draw()
{
	// Use the shader program for this object
	glUseProgram(this->shaderProgram);

	// Set Modeling transformation
	glUniformMatrix4fv(this->modelLocation, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));

	for (auto& subMesh : subMeshes) {

		// Bind vertex array object
		glBindVertexArray(subMesh.vao);

		SharedMaterialProperties::setShaderMaterialProperties(subMesh.modelMaterial);

		this->texture.setActive();

		// Fetch input data for pipeline	
		glDrawElements(this->primitiveMode, subMesh.indexCount, GL_UNSIGNED_INT, 0);

		SharedMaterialProperties::cleanUpMaterial(subMesh.modelMaterial);
	}

}