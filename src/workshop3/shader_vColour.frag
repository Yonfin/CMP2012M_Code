#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec3 Colour;			//input the Colour values from Vertex shader

void main()
{
	vertColour = vec4(Colour, 1.0);
}