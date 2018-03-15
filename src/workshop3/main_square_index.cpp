#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Triangle.h"
#include "Circle.h"
#include "Square.h"
#include "ShaderClass.h"
#include "GLerror.h"


// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>

#include "windows.h"

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


int mainSquare(int argc, char *argv[]) {
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

	//check for debug context
	GLint c;
	glGetIntegerv(GL_CONTEXT_FLAGS, &c);
	if (GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "OGL debug context loaded" << std::endl;
	}
	glEnable(GL_DEBUG_OUTPUT);

	//check the KHR_debug extension is supported
	if (GL_KHR_debug)
	{
		std::cout << "KHR_debug supported" << std::endl;
	}

	//initialise GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//set up error checking
	GLerror _error;
	int value = 0;

	//set up debug callback
	//register debug callback
	if (glDebugMessageCallback)
	{
		std::cout << "Registering OpenGL Debug callback function" << std::endl;
		glDebugMessageCallback(_error.openglCallbackFunction, &value);
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			NULL,
			true);
	}

	//create the square object
	Square square;

	//create shaders
	Shader vShc("..//src//shader_vColour.vert");
	Shader fShc("..//src//shader_vColour.frag");
	value = 1;

	//OpenGL specific values
	//****************************
	// OpenGL calls.


	//compile the shader code
	//1 for vertex, 2 for fragment
	vShc.getShader(1);
	fShc.getShader(2);
	value = 2;

	//
	//Create and link the Shader program
	//
	GLuint shaderProgram_colour;

	shaderProgram_colour = glCreateProgram();
	glAttachShader(shaderProgram_colour, vShc.shaderID);
	glAttachShader(shaderProgram_colour, fShc.shaderID);
	glLinkProgram(shaderProgram_colour);

	//delete shaders
	glDeleteShader(vShc.shaderID);
	glDeleteShader(fShc.shaderID);
	value = 3;

	//
	//OpenGL buffers
	//set buffers for the square
	square.setBuffers();
	value = 4;


	//A game loop, rendering, then handling input
	SDL_Event event;
	bool windowOpen = true;

	while (windowOpen)
	{

		//background
		glClearColor(0.3f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT);  //should be GL_COLOR_BUFFER_BIT
		value = 5;

		// Drawing code
		//set to wireframe so we can see the triangles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Draw the object
		glUseProgram(shaderProgram_colour);
		square.render();

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