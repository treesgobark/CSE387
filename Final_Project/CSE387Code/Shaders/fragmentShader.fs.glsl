#version 430 core

#pragma optimize(on)
#pragma debug(off)

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
//	bool normalMapEnabled;
//	bool bumpMapEnabled;
};

layout(shared) uniform MaterialBlock
{
	Material object;
};

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

layout(location = 100) uniform sampler2D diffuseSampler;
layout(location = 101) uniform sampler2D specularSampler;
//layout(location = 102) uniform sampler2D normalMapSampler;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;
in vec2 TexCoord;
in vec4 viewSpace;

out vec4 fragmentColor;

vec3 shadingCaculation(GeneralLight light, Material object);
vec3 fragmentWorldNormal;

const vec3 fogColor = vec3(0.2, 0.5, 0.8);
const float fogDensity = 0.03;

void main()
{

	// Make copy of material properties that can be written to
	Material material = object;

	fragmentWorldNormal = vertexWorldNormal;

	// Substitute diffuse texture for ambient and diffuse material properties
	if (material.diffuseTextureEnabled == true && material.textureMode != 0) {

		material.diffuseMat = texture(diffuseSampler, TexCoord.st);
		material.ambientMat = material.diffuseMat;
	}

	// Substitute specular texture for specular material properties
	if (material.specularTextureEnabled == true && material.textureMode != 0) {

		material.specularMat = texture(specularSampler, TexCoord.st);
	}

	// Should shading calculations be performed
	if (material.textureMode == 2 || material.textureMode == 0) {

		fragmentColor = material.emmissiveMat;

		for (int i = 0; i < MaxLights; i++) {

			fragmentColor += vec4(shadingCaculation(lights[i], material), 1.0);
		}

	}
	else if (material.textureMode == 1) { // No shading calculations

		fragmentColor = texture(diffuseSampler, TexCoord.st);
	}


	// fog calculations
	float dist = length(viewSpace);
	float distFactor = 1.0 / exp((dist * fogDensity) * (dist * fogDensity));
	distFactor = clamp(distFactor, 0.0, 1.0);

	fragmentColor = mix(vec4(fogColor, 1.0), fragmentColor, distFactor);


} // main


vec3 shadingCaculation(GeneralLight light, Material object)
{
	vec3 totalFromThisLight = vec3(0.0, 0.0, 0.0);

	if (light.enabled == true) {

		// Calculate a bunch of vectors
		vec3 lightVector;
		if (light.positionOrDirection.w < 1) {
			// Directional
			lightVector = normalize(light.positionOrDirection.xyz);
		}
		else {
			// Positional
			lightVector = normalize(light.positionOrDirection.xyz -
				vertexWorldPosition.xyz);
		}

		vec3 reflection = normalize(reflect(-lightVector, fragmentWorldNormal.xyz));
		vec3 eyeVector = normalize(worldEyePosition - vertexWorldPosition.xyz);

		float spotCosFGameObject = 0;
		if (light.isSpot == true) {

			spotCosFGameObject = dot(-lightVector, normalize(light.spotDirection));
		}

		// Is it a spot light and are we in the cone?
		if (light.isSpot == false || (light.isSpot == true && spotCosFGameObject >= light.spotCutoffCos)) {

			// Ambient Reflection
			totalFromThisLight += object.ambientMat.xyz * light.ambientColor.xyz;

			// Difuse Reflection
			totalFromThisLight += max(dot(fragmentWorldNormal.xyz, lightVector), 0.0f) * object.diffuseMat.xyz * light.diffuseColor.xyz;

			// Specular Reflection
			totalFromThisLight += pow(max(dot(reflection, eyeVector), 0.0f), object.specularExp) * object.specularMat.xyz * light.specularColor.xyz;
		}
	}

	return totalFromThisLight;

} // end shadingCaculation
