#version 420 core

layout (triangles, equal_spacing, ccw) in;

in vec3 tcPosition[];
in vec4 tcColor[];

out vec4 teColor;

uniform mat4 projectionMatrix;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   
{                                                                                               
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;   
}

void main(void)
{
	// Use Barycentri interpolation to create interpolated position and
	// calculate the position in clip coordinates
    gl_Position = projectionMatrix * vec4( interpolate3D( tcPosition[0],
														  tcPosition[1],
														  tcPosition[2] ) , 1.0);

	// Use Barycentri interpolation to create interpolated color
    teColor = vec4( interpolate3D( tcColor[0].xyz,
								   tcColor[1].xyz,
								   tcColor[2].xyz ) , 1.0);

} // end main

