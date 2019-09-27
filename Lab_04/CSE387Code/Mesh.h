#pragma once

#include "MathLibsConstsFuncs.h"
#include "BuildShaderProgram.h"
#include "Texture.h"


//#include "Material.h"

enum RENDER_MODE { ORDERED, INDEXED };

// Base class for all visual objects that will appear in the scene.
class Mesh 
{
public: 

	// Default constructor for early labs
	Mesh() {};

	// Destructor
	virtual~Mesh();

	// Buffer all the data that is associated with the vertex 
	// processor for this object. Creates a Vertex Array Object that
	// holds references to the vertex buffers, the index buffer and 
	// the layout specification. 
	virtual void initialize(GLuint shaderProgram) = 0;

	// Render the object and all of its children
	virtual void draw();

	// Modeling transform for this VisualObject
	// Should be protected. Temporarily left public for demonstration 
	// purposes
	mat4 modelMatrix;
	
	// Fixed transformation to orient and position this object within
	// its object coordinate frame. Not considered part of the modeling
	// transformation.
	//mat4 fixedTransformation;

	// Material for lighting calculations
	//Material material;

	virtual void setTexture(Texture tex);

protected:

	/**
	 * @brief	Matrix that includes both the modeling and any fixed transformation of the object
	 * 			This matrix is passed to the shader during rendering. Child of the object only
	 * 			transform relative to the modeling transformation of this object. Children are not
	 * 			aware of any fixed transformations that are used by the parent.
	 */
	glm::mat4 modelAndFixed;

	/**
	 * @brief	Encapsulates all the data that is associated with the vertex processor for this
	 * 			object. Instead of containing the actual data, it holds references to the vertex
	 * 			buffers, the index buffer and the layout specification of the vertex itself.
	 */
	GLuint VAO; 

	// Indentifier for the shader program
	GLuint shaderProgram = 0; 

	// Location in the vertex shader for the modeling transformation
	GLuint modelLocation;

	RENDER_MODE renderMode; 
	/**   
	 * @brief	Number of vertices or indices to be processed for either ordered or indexed rendering
	*/
	GLuint count = 0;

	Texture tex;

}; // end VisualObject class




