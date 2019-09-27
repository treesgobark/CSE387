// Targeting version 4.3 of GLSL. If the compiler does not support
// 4.3 it will cause an error.
#version 430 core

//layout(location = 0) uniform mat4 projectionMatrix;
//layout(location = 1) uniform mat4 viewMatrix;
//layout(location = 2) uniform mat4 modelMatrix;

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat4 normalModelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
};

layout (location = 0) in vec4 vertexPosition;
layout (location = 1) in vec4 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;


out vec2 TexCoord0;
out vec4 vertexNorm;

void main(void)
{
	// Calculate the position in clip coordinates
	gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vertexPosition;
	vertexNorm = normalModelMatrix * vertexNormal;
	TexCoord0 = vertexTexCoord;
}