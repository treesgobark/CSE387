#pragma once

#include "MathLibsConstsFuncs.h"

#include "SharedMaterialProperties.h"
#include "SharedProjectionAndViewing.h"
#include "btBulletDynamicsCommon.h"
#include "MathLibsConstsFuncs.h"

#include "Texture.h"

/**
 * @enum	RENDER_MODE
 *
 * @brief	Values that represent render modes to be used when a sub-mesh if rendered.
 */
enum RENDER_MODE { ORDERED, INDEXED };

/**
 * @struct	SubMesh
 *
 * @brief	Stuct containing values needed to render a sub-mesh. All visible objects
 * 			(Meshes) are rendered using one or more sub-meshes.
 */
struct SubMesh {

	GLuint vao = 0; // ID for Vertex Array Object for the sub-mesh

	GLuint vertexBuffer = 0; // ID for vertex data buffer for the sub-mesh

	GLuint indexBuffer = 0; // ID for index buffer for the sub-mesh (if indexed rendering is used)

	Material* material = nullptr; // Pointer to material properties including textures for the sub-mesh

	GLuint count = 0; // Either the number of vertices in the mesh or the number of indices

	RENDER_MODE renderMode = INDEXED; // Render mode for the mesh. Either ORDERED or INDEXED

	GLuint primitiveMode = GL_TRIANGLES; // Primite mode for the mesh GL_POINTS, GL_LINES, etc.
};


/**
 * @struct	pntVertexData
 *
 * @brief	Structure for holding vertex data for a single vertex.
 */
struct pntVertexData
{
	glm::vec3 m_pos = ZERO_V3; // Position of the vertex in Object coordinates
	glm::vec3 m_normal = ZERO_V3; // Normal vector for the vertex in Object coordinates
	glm::vec2 m_textCoord = ZERO_V2; // 2D vertex texture coordinates

	pntVertexData() {}

	pntVertexData(glm::vec3 pos, glm::vec3 normal, glm::vec2 textCoord)
	{
		m_pos = pos;
		m_normal = normal;
		m_textCoord = textCoord;
	}
};


/**
 * @class	Mesh
 *
 * @brief	Base class for all objects that will appear visually in the scene. All meshes
 * 			are rendered as one or more sub-meshes.
 */
class Mesh : public Component
{
public: 

	// Default constructor for early labs
	Mesh() {};

	// Destructor
	virtual~Mesh();

	/**
	 * @fn	virtual void Mesh::initialize(GLuint shaderProgram) = 0;
	 *
	 * @brief	Buffer all the data that is associated with the vertex processor for this object.
	 * 			Creates a Vertex Array Object that holds references to the vertex buffers, the index
	 * 			buffer and the layout specification.
	 *
	 * @param	shaderProgram	Shader program the will be used to render the object.
	 */
	virtual void initialize(GLuint shaderProgram) = 0;

	/**
	 * @fn	virtual void Mesh::draw();
	 *
	 * @brief	Renders all sub-meshes that are part of the object.
	 */
	virtual void draw();

	// Modeling transform for this VisualObject
	// Should be protected. Temporarily left public for demonstration 
	// purposes
	mat4 modelMatrix;

	// Location in the vertex shader for the modeling transformation
	// Not needed when SharedProjectionAndViewing is used
	GLuint modelLocation = 2;

protected:

	/**
	 * @fn	SubMesh Mesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material);
	 *
	 * @brief	Builds a sub-mesh for indexed rendering
	 *
	 * @param 		  	vertexData	Information describing the vertex.
	 * @param 		  	indices   	The indices used for indexed rendering.
	 * @param [in,out]	material  	If non-null, the material.
	 *
	 * @returns	the SubMesh that was constructed.
	 */
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material);

	/**
	 * @fn	SubMesh Mesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, Material* material);
	 *
	 * @brief	Builds sub-mesh for ordered rendering
	 *
	 * @param 		  	vertexData	Information describing the vertex.
	 * @param 		  	indices   	The indices used for indexed rendering.
	 * @param [in,out]	material  	If non-null, the material.
	 *
	 * @returns	the SubMesh that was constructed.
	 */			
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, Material* material);

	/** @brief	Indentifier for the shader program used to render all sub-meshes */
	GLuint shaderProgram = 0; 

	/**
	 * @class	btCollisionShape*
	 *
	 * @brief	Point to bt collision shape* that can be used by the Physics Engine
	 * 			for collision detection.
	 */
	class btCollisionShape* collisionShape;

	/** @brief	Container for all sub meshes that are part of this object.*/
	std::vector<SubMesh> subMeshes;

}; // end VisualObject class




