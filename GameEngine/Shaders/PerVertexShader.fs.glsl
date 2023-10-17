// Targeting version 4.5 of GLSL. 
#version 460 core

in vec4 vsColor;
out vec4 fragmentColor;

void main()
{
	fragmentColor = vsColor;
}