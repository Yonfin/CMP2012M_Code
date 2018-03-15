//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//
//// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
//// // NOTE: include before SDL.h
//#include <GL/glew.h>
//#include "windows.h"
//
//// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
//#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"
//
//// // - OpenGL Mathematics - https://glm.g-truc.net/
//#define GLM_FORCE_RADIANS // force glm to use radians
//// // NOTE: must do before including GLM headers
//// // NOTE: GLSL uses radians, so will do the same, for consistency
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//
//int main(int argc, char *argv[]) {
//	// SDL initialise
//	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
//			SDL_GetError());
//		return 1;
//	}
//	SDL_Log("SDL initialised OK!\n");
//
//	// Window Creation - modified for OpenGL
//	SDL_Window *win = nullptr;
//	win = SDL_CreateWindow("OpenGL Window", 100, 100, 800, 600,
//		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
//	if (win == nullptr) {
//		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
//			"SDL_CreateWindow init error: %s\n", SDL_GetError());
//		return 1;
//	}
//	 
//	//set context attributes
//	int major = 4, minor = 3;
//
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE); //use core profile
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //ask for forward compatible
//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //default, probably
//
//	// Create an OpenGL context associated with the window.
//	SDL_GLContext glcontext = SDL_GL_CreateContext(win);
//
//	if (glcontext == NULL) {
//		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
//			"SDL_GL_CreateContext init error: %s\n", SDL_GetError());
//		return 1;
//	}
//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
//	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
//	SDL_Log("Got an OpenGL %d.%d context\n", major, minor);
//
//	//check the KHR_debug extension is supported
//	if (GL_KHR_debug)
//	{
//		std::cout << "KHR_debug supported" << std::endl;
//	}
//	//check for debug context
//	GLint c;
//	glGetIntegerv(GL_CONTEXT_FLAGS, &c);
//	if (GL_CONTEXT_FLAG_DEBUG_BIT)
//	{
//		std::cout << "OGL debug context loaded" << std::endl;
//	}
//
//	glewExperimental = GL_TRUE;
//	GLenum err = glewInit();
//
//	std::cout << "OpenGL version is " << glGetString(GL_VERSION) << std::endl;
//
//	
//
//	//****************************
//	// OpenGL calls.
//	
//	float redVal = 0.0f;
//	float blueVal = 0.0f;
//	float greenVal = 0.0f;
//
//	glClearColor(1.0f, 0.0f, 0.0f, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//	SDL_GL_SwapWindow(win);
//
//	//*****************************
//	//SDL handled input
//	//Any input to the program is done here
//
//	SDL_Event event;
//	bool windowOpen = true;
//
//	while (windowOpen)
//	{
//		redVal == redVal + 0.01f;
//
//		if (redVal == 1.0f)
//		{
//			redVal = 0.0f;
//		}
//
//		if (SDL_PollEvent(&event))
//		{
//			if (event.type == SDL_QUIT)
//			{
//				windowOpen = false;
//			}
//
//			if (event.key.keysym.sym == SDLK_ESCAPE)
//			{
//				windowOpen = false;
//			}
//
//			if (event.key.keysym.sym == SDLK_z)
//			{
//				glClearColor(1.0f, 0.0f, 0.0f, 1);
//				glClear(GL_COLOR_BUFFER_BIT);
//				SDL_GL_SwapWindow(win);
//			}
//
//			if (event.key.keysym.sym == SDLK_x)
//			{
//				glClearColor(0.0f, 0.0f, 1.0f, 1);
//				glClear(GL_COLOR_BUFFER_BIT);
//				SDL_GL_SwapWindow(win);
//			}
//
//			if (event.key.keysym.sym == SDLK_c)
//			{
//				glClearColor(0.0f, 1.0f, 0.0f, 1);
//				glClear(GL_COLOR_BUFFER_BIT);
//				SDL_GL_SwapWindow(win);
//			}
//		}
//	}
//
//	// Clean up
//	SDL_Log("Finished. Cleaning up and closing down\n");
//
//	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
//	SDL_GL_DeleteContext(glcontext);
//
//	SDL_Quit();
//	return 0;
//}
