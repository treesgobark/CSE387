#version 430 core

#pragma optimize(on)
#pragma debug(off)

layout(shared) uniform transformBlock
{
	mat4 modelMatrix;
	mat3 normalModelMatrix;
	mat4 viewingMatrix;
	mat4 projectionMatrix;
};


out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;
out vec2 TexCoord;

layout (location = 0) in vec3 vertexPosition; 
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

void main()
{
	// Make a vec4 version of the vertexPosition in object coords
	vec4 vPos = vec4(vertexPosition, 1.0f);

    // Transform the position of the vertext to clip coordinates
    gl_Position = projectionMatrix * viewingMatrix * modelMatrix * vPos;

	// Transform the position of the vertex to world coords for lighting
	vertexWorldPosition = vec3(modelMatrix * vPos);

	// Transform the normal to world coords for lighting
	vertexWorldNormal = normalize(normalModelMatrix * vertexNormal);

	// Pass through the texture coordinate
	TexCoord = vertexTexCoord; 

} // end main

