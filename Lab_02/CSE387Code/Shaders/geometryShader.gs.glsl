#version 420 core

layout (triangles) in; // input format
layout (triangle_strip, max_vertices = 3) out; // output format

in vec4 teColor[];

out vec4 gsColor;

void main(void)
{

    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
		gsColor = teColor[i];
        EmitVertex();
    }

} // end main