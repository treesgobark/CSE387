#version 430 core

#pragma optimize(on)
#pragma debug(off)

layout(location = 2) uniform mat4 modelMatrix;
layout(location = 1) uniform mat4 viewingMatrix;
layout(location = 0) uniform mat4 projectionMatrix;
layout(location = 4) uniform mat3 normalModelMatrix;

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

