#include <iostream>

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#include <GL/glew.h>

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

int main(int argc, char *argv[]) {
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

	//OpenGL specific values
	//add second tri vertices to array
	GLfloat vertices[] = {
		-0.9f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.45f,  0.5f, 0.0f,
		// Second triangle
		0.0f, -0.5f, 0.0f,
		0.9f, -0.5f, 0.0f,
		0.7f, 0.5f, 0.0f
	};

	std::cout << "OpenGL version is" << glGetString(GL_VERSION) << std::endl;

	//****************************
	// OpenGL calls.
	GLenum err = glewInit();
	//vertex shader
	GLchar const *vShader = { "#version 440 core\nlayout(location=0) in vec3 position;\nvoid main()\n{ gl_Position = vec4(position.x, position.y, position.z, 1.0);}\n" };

	GLchar const *fShader = { "#version 440 core\nout vec4 color;\nvoid main()\n{ color = vec4(1.0f, 1.0f, 0.0f, 1.0f);}\n" };

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShader, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShader, NULL);
	glCompileShader(fragmentShader);

	//debug shader compile
	GLint vsuccess;
	GLint fsuccess;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vsuccess);
	std::cout << "vertexShader compile: " << vsuccess << std::endl;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fsuccess);
	std::cout << "fragmentShader compile: " << fsuccess << std::endl;

	if (!vsuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	if (!fsuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//
	//Shader program
	//
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//debug shader program compile
	GLint spsuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &spsuccess);
	std::cout << "shaderProgram compile: " << spsuccess << std::endl;
	if (!spsuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//delete shaders code
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//
	//OpenGL buffers
	//set up for the shapes
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	//bind the Vertex Array Object (VAO)
	glBindVertexArray(VAO);
	//copy the vertices array into a buffer (VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Unbind the VAO
	glBindVertexArray(0);


	// Drawing code
	//set background colour
	glClearColor(1.0f, 0.0f, 0.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	//draw the triangles
	//Use shader program we have compiled and linked
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	//set to wireframe so we can see the 2 triangles
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//drawing 6 vertices (2 triangles)
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	SDL_GL_SwapWindow(win);


	//close window when we want
	SDL_Event event;
	bool windowOpen = true;

	while (windowOpen)
	{
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