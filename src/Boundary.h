#pragma once
#include <GL/glew.h>
#include <array>
#include "Globals.h"

class Boundary
{
public:
	//constructor
	Boundary(int inbound) 
	{
		bound = inbound;
	};
	//define vertices for the square
	GLfloat top_vertices[32] = {
		//vertices									colours				texture coordinates
		-0.77f*aspectRatio, 0.95f, 0.0f,			0.0f, 0.5f, 0.0f,   1.0f, 1.0f,
		 0.77f*aspectRatio, 0.95f, 0.0f,			0.5f, 0.0f, 0.0f,   1.0f, 0.0f,
		 0.77f*aspectRatio,  1.0f, 0.0f,			0.0f, 0.0f, 0.5f,   0.0f, 0.0f,
		-0.77f*aspectRatio,  1.0f, 0.0f,			0.5f, 0.0f, 0.5f,   0.0f, 1.0f
	};
	GLfloat left_vertices[32] = {
		//vertices									colours				texture coordinates
		-0.77f*aspectRatio, -1.0f, 0.0f,			0.0f, 0.5f, 0.0f,   1.0f, 1.0f,
		-0.72f*aspectRatio, -1.0f, 0.0f,			0.5f, 0.0f, 0.0f,   1.0f, 0.0f,
		-0.72f*aspectRatio,  1.0f, 0.0f,			0.0f, 0.0f, 0.5f,   0.0f, 0.0f,
		-0.77f*aspectRatio,  1.0f, 0.0f,			0.5f, 0.0f, 0.5f,   0.0f, 1.0f
	};
	GLfloat right_vertices[32] = {
		//vertices									colours				texture coordinates
		 0.77f*aspectRatio,	-1.0f, 0.0f,			0.0f, 0.5f, 0.0f,   1.0f, 1.0f,
		 0.82f*aspectRatio, -1.0f, 0.0f,			0.5f, 0.0f, 0.0f,   1.0f, 0.0f,
		 0.82f*aspectRatio,  1.0f, 0.0f,			0.0f, 0.0f, 0.5f,   0.0f, 0.0f,
		 0.77f*aspectRatio,  1.0f, 0.0f,			0.5f, 0.0f, 0.5f,   0.0f, 1.0f
	};
	int bound;
	//define indices for the square
	GLuint indices[6] = 
	{
		0, 1, 3,
		1, 2, 3
	};

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	GLuint EBO;

	void setBuffers()
	{
		//OpenGL buffers
		//set up 1 for the triangle
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		//initialise the index buffer
		glGenBuffers(1, &EBO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		if (bound == 0)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(top_vertices), top_vertices, GL_STATIC_DRAW);
		}
		if (bound == 1)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(left_vertices), left_vertices, GL_STATIC_DRAW);
		}
		if (bound == 2)
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(right_vertices), right_vertices, GL_STATIC_DRAW);
		}
		//set up the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		//set the colour attribute pointer
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		//set texture attrib pointer
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		//Unbind the VAO
		glBindVertexArray(0);
	}

	void render()
	{
		//draw the square 
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

};