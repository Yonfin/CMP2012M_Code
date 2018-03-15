#version 440 core
layout (location = 0) in vec3 Position; //vertex positions
layout (location=1) in vec3 vColour;	//vertex colours

out vec3 Colour; 						//output vertex colour to fragment shader

void main()
{
	gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
	Colour = vColour; 					//pass the vertex colour unchanged to the fragment shader

}