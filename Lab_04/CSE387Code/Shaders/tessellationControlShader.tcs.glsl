#version 420 core

layout (vertices = 3) out;

in vec3 vsPosition[];
in vec4 vsColor[]; 

out vec3 tcPosition[];
out vec4 tcColor[];

void main()
{
	// Specify sub-divisions for the triangles
    gl_TessLevelInner[0] = 1.0;
    gl_TessLevelOuter[0] = 1.0;
    gl_TessLevelOuter[1] = 1.0;
    gl_TessLevelOuter[2] = 1.0;

    // Copy input to output
    tcPosition[gl_InvocationID]= vsPosition[gl_InvocationID];
	tcColor[gl_InvocationID] = vsColor[gl_InvocationID];

} // end main

