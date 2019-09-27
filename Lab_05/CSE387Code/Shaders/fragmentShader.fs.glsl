// Targeting version 4.3 of GLSL. If the compiler does not support 
// 4.3 it will cause an error.
#version 430 core
#pragma optimize(off)
#pragma debug(on)

const int MaxLights = 8;

// Structure for holding general light properties
struct GeneralLight
{

	vec4 ambientColor;		// ambient color of the light
	vec4 diffuseColor;		// diffuse color of the light
	vec4 specularColor;		// specular color of the light

							// Either the position or direction
							// if w = 0 then the light is directional and direction specified 
							// is the negative of the direction the light is shinning
							// if w = 1 then the light is positional
	vec4 positionOrDirection;

	// Spotlight attributes
	vec3 spotDirection;		// the direction the cone of light is shinning    
	bool isSpot;				// 1 if the light is a spotlight  
	float spotCutoffCos;	// Cosine of the spot cutoff angle
	float spotExponent;		// For gradual falloff near cone edge

	// Attenuation coefficients
	float constant;
	float linear;
	float quadratic;

	bool enabled;			// true if light is "on"

};

layout(shared) uniform LightBlock
{
	GeneralLight lights[MaxLights];
};


struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
	int textureMode;
	bool diffuseTextureEnabled;
	bool specularTextureEnabled;
	bool normalMapEnabled;
	bool bumpMapEnabled;
};

layout(shared) uniform MaterialBlock
{
	Material object;
};

layout (shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

layout(location = 100) uniform sampler2D diffuseSampler;
layout(location = 101) uniform sampler2D specularSampler;
layout(location = 102) uniform sampler2D normalMapSampler;
layout(location = 103) uniform sampler2D bumpMapSampler;

in vec2 TexCoord0;
out vec4 fragmentColor;
uniform vec4 objectColor;
uniform sampler2D sampler;

void main()
{
	//fragmentColor = texture( sampler, TexCoord0.st );
	//vec4 texelColor = objectColor * texture( sampler, TexCoord0.st );
	//fragmentColor = objectColor * (1 - texelColor.a) + texelColor * texelColor.a;
	//fragmentColor.a = objectColor.a;
	//fragmentColor = lights[0].diffuseColor * lights[0].ambientColor * object.ambientMat;
	fragmentColor = lights[0].specularColor * object.specularMat;
}