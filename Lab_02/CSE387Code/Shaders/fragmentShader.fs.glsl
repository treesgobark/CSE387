// Targeting version 4.3 of GLSL. If the compiler does not support 
// 4.3 it will cause an error.
#version 430 core


in vec4 vertexColor;
in vec2 TexCoord0;
out vec4 fragmentColor;

uniform sampler2D sampler;

void main()
{
	fragmentColor = vertexColor;

	// fragmentColor = texture( sampler, TexCoord0.st );
}