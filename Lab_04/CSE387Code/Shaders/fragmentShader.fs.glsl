// Targeting version 4.3 of GLSL. If the compiler does not support 
// 4.3 it will cause an error.
#version 430 core

struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
};

Material object;

in vec2 TexCoord0;
out vec4 fragmentColor;
uniform vec4 objectColor;
uniform sampler2D sampler;
layout (location = 100) uniform sampler2D diffuseSampler;
layout (location = 101) uniform sampler2D specularSampler;

void main()
{
	fragmentColor = texture( sampler, TexCoord0.st );
	//vec4 texelColor = objectColor * texture( sampler, TexCoord0.st );
	//fragmentColor = objectColor * (1 - texelColor.a) + texelColor * texelColor.a;
	//fragmentColor.a = objectColor.a;
}