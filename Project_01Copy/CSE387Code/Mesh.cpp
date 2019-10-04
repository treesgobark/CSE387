#include "Mesh.h"

Mesh::~Mesh()
{ 
	for (auto& subMesh : subMeshes) {

		glDeleteVertexArrays(1, &subMesh.vao);

		glDeleteBuffers(1, &subMesh.vertexBuffer);

		if (subMesh.renderMode == INDEXED) {
			glDeleteBuffers(1, &subMesh.indexBuffer);
		}

		delete subMesh.material;
	}

} // end destructor


// Preform drawing operations. Draw this object and all of its children.
void Mesh::draw()
{
	// Use the shader program for this object
	glUseProgram(this->shaderProgram);

	// Set Modeling transformation
	//glUniformMatrix4fv(this->modelLocation, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));

	SharedProjectionAndViewing::setModelingMatrix(this->modelMatrix);

	for (auto& subMesh : subMeshes) {

		// Bind vertex array object
		glBindVertexArray(subMesh.vao);

		// Set the material properties in the shader program
		SharedMaterialProperties::setShaderMaterialProperties(subMesh.material);

		if (subMesh.renderMode == ORDERED) {

			// Fetch input data for pipeline	
			glDrawArrays(subMesh.primitiveMode, 0, subMesh.count);

		}
		else { // renderMode == INDEXED

			// Fetch input data for pipeline	
			glDrawElements(subMesh.primitiveMode, subMesh.count, GL_UNSIGNED_INT, 0);
		}

		SharedMaterialProperties::cleanUpMaterial(subMesh.material);
	}

} // end draw

SubMesh Mesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material)
{
	SubMesh subMesh;

	// Store the address of the material struct for the submesh
	subMesh.material = material;

	// Generate and bind the vertex array object
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	// Generate, bind, and load the vertex array object
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

	// Generate, bind, and buffer the indices in the Index Array Buffer
	subMesh.count = static_cast<unsigned int>(indices.size());
	glGenBuffers(1, &subMesh.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	subMesh.renderMode = INDEXED;

	return subMesh;
}

SubMesh Mesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, Material* material)
{
	SubMesh subMesh;

	// Store the address of the material struct for the submesh
	subMesh.material = material;

	// Generate and bind the vertex array object
	glGenVertexArrays(1, &subMesh.vao);
	glBindVertexArray(subMesh.vao);

	// Generate, bind, and load the vertex array object
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

	subMesh.renderMode = ORDERED;

	return subMesh;
}









