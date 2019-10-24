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
	bool normalMapEnabled;
	bool bumpMapEnabled;
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

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;
in vec2 TexCoord;

out vec4 fragmentColor;

vec3 fragmentWorldNormal;

vec3 shadingCalculation(GeneralLight light, Material material) {
	if (!light.enabled) return vec3(0.0);
	float ambientIntensity = 0.2;
	float diffuseIntensity = 1.0;
	float specularIntensity = 0.1;

	vec3 ambient = ambientIntensity * light.ambientColor.xyz * material.ambientMat.xyz;

	vec3 lightVector = normalize(light.positionOrDirection.xyz);
	vec3 lightDirection = vec3(0.0f);

	if (light.positionOrDirection.w == 0.0f)
		lightDirection = lightVector;
	else
		lightDirection = normalize(lightVector - vertexWorldPosition);
	
	float difference = max(dot(vertexWorldNormal, lightDirection), 0.0f);
	vec3 diffuse = diffuseIntensity * difference * light.diffuseColor.xyz * material.diffuseMat.xyz;

	vec3 viewingDirection = normalize(worldEyePosition - vertexWorldPosition);
	vec3 reflectionDirection = reflect(-lightDirection, vertexWorldNormal);
	float strength = pow(max(dot(viewingDirection, reflectionDirection), 0.0), 2);
	vec3 specular = specularIntensity * strength * light.specularColor.xyz * material.specularMat.xyz;

	if (!light.isSpot) return ambient + diffuse + specular;

	float theta = dot(lightDirection, normalize(-light.spotDirection));
	float epsilon = light.spotCutoffCos*.1;
	float spotIntensity = clamp((theta - light.spotCutoffCos) / epsilon, 0.0, 1.0);
	if (theta > light.spotCutoffCos) return ambient + spotIntensity * diffuse + spotIntensity * specular;
	return ambient;
}

void main()
{
	// make copy of material properties that can be written to
	Material material = object;

	// Substitute diffuse texture for ambient and diffuse material properties
	if (material.diffuseTextureEnabled == true && material.textureMode != 0) {

		material.diffuseMat = texture(diffuseSampler, TexCoord.st);
		material.ambientMat = material.diffuseMat;
	}

	// Substitute specular texture for specular material properties
	if (material.specularTextureEnabled == true && material.textureMode != 0) {

		material.specularMat = texture(specularSampler, TexCoord.st);
	}

	// Check if shading calculations should be performed
	if (material.textureMode == 2 || material.textureMode == 0) {

		fragmentColor = material.emmissiveMat;

		for (int i = 0; i < MaxLights; i++) {

			fragmentColor += vec4(shadingCalculation(lights[i], material), 1.0f);
		}

	}
	else if (material.textureMode == 1) { // No shading calculations

		fragmentColor = texture(diffuseSampler, TexCoord.st);
}


} // main
