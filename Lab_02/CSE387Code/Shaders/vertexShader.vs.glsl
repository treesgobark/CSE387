// Targeting version 4.3 of GLSL. If the compiler does not support 
// 4.3 it will cause an error.
#version 430 core

mat4 projectionMatrix; 
mat4 viewMatrix; 
mat4 modelMatrix;

out vec4 vertexColor;

// Declare a hard-coded array of positions
//const vec4 vertices[3] = vec4[3](vec4(-0.25, -0.25, 0.0, 1.0),
//		   						   vec4(0.25, -0.25, 0.0, 1.0),
//                                 vec4(0.25, 0.25, 0.0, 1.0));

// Declare a hard-coded array of colors
//const vec4 colors[3] = vec4[3](vec4(0.9, 0.1, 0.1, 1.0),
//                               vec4(0.1, 0.9, 0.1, 1.0),
//                               vec4(0.1, 0.1, 0.9, 1.0));

void main(in gl_Position, out fragmentColor)
{
	// Calculate the position in clip coordinates
    // gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);

     // Index into our array using gl_VertexID
     gl_Position = gl_Position;
	 vertexColor = fragmentColor;

}