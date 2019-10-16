#pragma once

#include "MathLibsConstsFuncs.h"
#include "Component.h"
#include "SharedMaterialProperties.h"
#include "SharedProjectionAndViewing.h"
#include "btBulletDynamicsCommon.h"
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

	GLuint vao = GL_INVALID_VALUE; // ID for Vertex Array Object for the sub-mesh

	GLuint vertexBuffer = GL_INVALID_VALUE; // ID for vertex data buffer for the sub-mesh

	GLuint indexBuffer = GL_INVALID_VALUE; // ID for index buffer for the sub-mesh (if indexed rendering is used)

	Material* material = nullptr; // Pointer to material properties including textures for the sub-mesh

	GLuint count = 0; // Either the number of vertices in the mesh or the number of indices

	RENDER_MODE renderMode = INDEXED; // Render mode for the mesh. Either ORDERED or INDEXED

	GLuint primitiveMode = GL_TRIANGLES; // Primite mode for the mesh GL_POINTS, GL_LINES, etc.

}; // end SubMesh


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
class MeshComponent : public Component
{
public: 

	/**
	 * @fn	MeshComponent::MeshComponent(GLuint shaderProgram, int updateOrder = 100)
	 *
	 * @brief	Constructor
	 *
	 * @param	shaderProgram	The shader program that will be used to render the mesh.
	 * @param	updateOrder  	(Optional) The update order. Determine the initialization
	 * 							 and update order of the component.
	 */
	MeshComponent(GLuint shaderProgram, int updateOrder = 100)
		: Component(updateOrder), shaderProgram(shaderProgram) {};

	/**
	 * @fn	MeshComponent::virtual~MeshComponent();
	 *
	 * @brief	Destructor. Deletes all dynamically allocated data.
	 */
	virtual~MeshComponent();

	/**
	 * @fn	virtual void Mesh::initialize();
	 *
	 * @brief	Buffers all the data that is associated with the vertex processor for this object.
	 * 			Creates a Vertex Array Object that holds references to the vertex buffers, the index
	 * 			buffer and the layout specification. This is a pure virtual method and must be
	 * 			overridden.
	 */
	virtual void initialize() override = 0;

	/**
	 * @fn	virtual void Mesh::draw();
	 *
	 * @brief	Renders all sub-meshes that are part of the object. Binds
	 * 			the vertex array object, sets the material properties, and sets
	 * 			the modeling transformation based on the world transformation
	 * 			of the owning game object.
	 */
	virtual void draw();

	/**
	 * @fn	virtual bool MeshComponent::isMesh() override
	 *
	 * @brief	Enables GameObject AddComponent to quickly
	 * 			and efficiently determine if the component being
	 * 			added is a mesh and will need to be rendered on
	 * 			frame updates.
	 * 			
	 * @returns	True if mesh, false if not.
	 */
	virtual bool isMesh() override { return true; }

protected:

	/**
	 * @fn	SubMesh MeshComponent::buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material);
	 *
	 * @brief	Builds one sub mesh  that will be rendered using indexed rendering based 
	 * 			the vertex data, indices, and material properties that are passed to it. 
	 * 			Both the vertex data and the indices are loaded into buffers located in 
	 * 			GPU memory.
	 *
	 * @param 		  	vertexData	Information describing the vertex.
	 * @param 		  	indices   	indices that will be used for indexed rendering
	 * @param			material  	If non-null, the material.
	 *
	 * @returns	A SubMesh.
	 */
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, const std::vector<unsigned int>& indices, Material* material);

	/**
	 * @fn	SubMesh Mesh::buildSubMesh(const std::vector<pntVertexData>& vertexData, Material* material);
	 * 		
	 * @brief	Builds one sub mesh  that will be rendered using sequential rendering based
	 * 			the vertex data, indices, and material properties that are passed to it.
	 * 			The vertex data is loaded into a buffer located in GPU memory.
	 *
	 * @param 		  	vertexData	Information describing the vertex.
	 * @param [in,out]	material  	If non-null, the material.
	 *
	 * @returns	A SubMesh.
	 */
	SubMesh buildSubMesh(const std::vector<pntVertexData>& vertexData, Material* material);

	/** @brief	Indentifier for the shader program used to render all sub-meshes */
	GLuint shaderProgram = 0; 


	/**
	 * @class	btCollisionShape*
	 *
	 * @brief	Pointer to bt collision shape that can be used by the Physics Engine
	 * 			for collision detection. The collision shape is based on vertex data
	 * 			in the rendered sub meshes.
	 */
	class btCollisionShape* collisionShape = NULL;

	/** @brief	Container for all sub meshes that are part of this component.*/
	std::vector<SubMesh> subMeshes;

}; // end VisualObject class




