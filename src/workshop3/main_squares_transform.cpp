#include <iostream>
#include <random>
#include "Triangle.h"
#include "Circle.h"
#include "Square.h"
#include "ShaderClass.h"
#include "TextureClass.h"

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>



// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int mainSqTrans(int argc, char *argv[]) {
	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");

	// Window Creation - modified for OpenGL
	SDL_Window *win = nullptr;
	win = SDL_CreateWindow("OpenGL Window", 100, 100, 800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", SDL_GetError());
		return 1;
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);

	std::cout << "OpenGL version is" << glGetString(GL_VERSION) << std::endl;


	//create objects
	Square square;

	//create texture
	Texture brick1;
	brick1.load("..//content//bricks.jpg");
	//set buffers for texture
	brick1.setBuffers();


	//create shaders
	Shader vSh("..//src//shader_vColour_Transform.vert");
	Shader fSh("..//src//shader_vColour_Transform.frag");

	//OpenGL specific values
	//****************************
	// OpenGL calls.
	GLenum err = glewInit();


	//compile the shader code
	//1 for vertex, 2 for fragment
	vSh.getShader(1);
	fSh.getShader(2);

	//
	//Create and link the Shader program
	//
	GLuint shaderProgram;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	//delete shaders
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);

	//set uniform variables
	int transformLocation;

	//OpenGL buffers
	//set buffers for square
	square.setBuffers();


	//A game loop, rendering, then handling input
	SDL_Event event;
	bool windowOpen = true;
	int counter = 0;

	//transform matrices 
	glm::mat4 translate;
	glm::mat4 rotate;
	translate = glm::translate(translate, glm::vec3(0.6f, -0.4f, 0.0f));
	rotate = glm::rotate(rotate, glm::radians(45.0f), glm::vec3(0, 0, 1));



	while (windowOpen)
	{

		//background
		glClearColor(0.9f, 0.9f, 0.9f, 1);
		glClear(GL_COLOR_BUFFER_BIT);  //should be GL_COLOR_BUFFER_BIT


									   // Drawing code
									   //set to wireframe so we can see the triangles
									   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

									   // Draw the object
		glUseProgram(shaderProgram);

		//transform uniforms
		transformLocation = glGetUniformLocation(shaderProgram, "uTransform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(translate*rotate));

		//bind the texture we want to use
		glBindTexture(GL_TEXTURE_2D, brick1.texture);

		//Draw the square - call its render method

		square.render();


		counter++;
		SDL_GL_SwapWindow(win);

		//Process input
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				windowOpen = false;
			}
		}
	}

	// Clean up
	SDL_Log("Finished. Cleaning up and closing down\n");

	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;
}


#endif