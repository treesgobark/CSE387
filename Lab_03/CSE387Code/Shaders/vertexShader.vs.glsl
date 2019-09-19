// Targeting version 4.3 of GLSL. If the compiler does not support
// 4.3 it will cause an error.
#version 430 core

layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 modelMatrix;

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec4 vertexColorIn;

out vec4 vertexColor;

// Declare a hard-coded array of positions
const vec4 vertices[3] = vec4[3](vec4(-0.25, -0.25, 0.0, 1.0),
		   						 vec4(0.25, -0.25, 0.0, 1.0),
                                 vec4(0.25, 0.25, 0.0, 1.0));

// Declare a hard-coded array of colors
const vec4 colors[3] = vec4[3](vec4(0.9, 0.1, 0.1, 1.0),
                               vec4(0.1, 0.9, 0.1, 1.0),
                               vec4(0.1, 0.1, 0.9, 1.0));

void main(void)
{
	// Calculate the position in clip coordinates
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vertexPosition;
	vertexColor = vertexColorIn;
}