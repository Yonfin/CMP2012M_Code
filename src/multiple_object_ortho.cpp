#include <iostream>
#include "Triangle.h"
#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include <random>

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

Texture* texArray;

int main(int argc, char *argv[]) {
	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");

	// Window Creation - modified for OpenGL
	SDL_Window *win = nullptr;
	win = SDL_CreateWindow("OpenGL Window", 100, 100, 800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) 
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", SDL_GetError());
		return 1;
	}

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);

	std::cout << "OpenGL version is" << glGetString(GL_VERSION) << std::endl;


	//create objects
	int objectCount = 5;
	Square squares[11][9];
	Square background;
	Square paddle;

	//create textures
	texArray = new Texture[17];

	//temp textures
	texArray[0].load("..//content//up.png");
	texArray[0].setBuffers();
	texArray[1].load("..//content//circlePattern.png");
	texArray[1].setBuffers();
	texArray[2].load("..//content//bricks.png");
	texArray[2].setBuffers();

	//background textures
	texArray[3].load("..//content//sky.png");
	texArray[3].setBuffers();
	texArray[4].load("..//content//xpgrass.png");
	texArray[4].setBuffers();
	
	//loading in the block textures
	texArray[5].load("..//content//Green Block.png");
	texArray[5].setBuffers();
	texArray[6].load("..//content//Red Block.png");
	texArray[6].setBuffers();
	texArray[7].load("..//content//Purple Block.png");
	texArray[7].setBuffers();
	texArray[8].load("..//content//Yellow Block.png");
	texArray[8].setBuffers();
	texArray[9].load("..//content//Blue Block.png");
	texArray[9].setBuffers();
	texArray[10].load("..//content//Misty Blue Block.png");
	texArray[10].setBuffers();
	texArray[11].load("..//content//Orange Block.png");
	texArray[11].setBuffers();
	texArray[12].load("..//content//Grey Block.png");
	texArray[12].setBuffers();
	texArray[13].load("..//content//Dark Green Block.png");
	texArray[13].setBuffers();
	texArray[14].load("..//content//Black Block.png");
	texArray[14].setBuffers();
	texArray[15].load("..//content//Cyan Block.png");
	texArray[15].setBuffers();

	//player paddle
	texArray[16].load("..//content//Paddle.png");
	texArray[16].setBuffers();

	//create shaders
	Shader vSh("..//src//shader_projection.vert");
	Shader fSh("..//src//shader_projection.frag");

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
	int modelLocation;
	int viewLocation;
	int projectionLocation;

	glUseProgram(shaderProgram);

	//essential to enable alpha in openGL
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//OpenGL buffers
	//set buffers for shapes
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			squares[i][j].setBuffers();
		}
	}
	background.setBuffers();
	paddle.setBuffers();

	//tranform matrices 
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 mTranslate;
	glm::mat4 mRotate;
	glm::mat4 mScale;
	glm::mat4 backgroundScale;
	glm::mat4 backgroundTranslate;
	glm::mat4 paddleTranslate;
	glm::vec3 scaleFactor;
	glm::vec3 b_scaleFactor;

	//once only scale to square
	scaleFactor = { 1.0f, 0.4f, 1.0f };
	mScale = glm::scale(mScale, glm::vec3(scaleFactor));

	//once only scale to background
	b_scaleFactor = { 10.0f, 10.0f, 1.0f };
	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, 0.0f));

	//set initial position of paddle
	paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, -0.95f, 0.0f));

	//set view matrix to identity
	//matrix equivalent of '1'
	viewMatrix = glm::mat4(1.0f);

	//orthographic projection
	//left, right, bottom, top, near clip plane, far clip plane
	//set width and height values
	projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);


	//A game loop, rendering, then handling input
	SDL_Event event;
	bool windowOpen = true;

	while (windowOpen)
	{

		//background colour
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);  //should be GL_COLOR_BUFFER_BIT

									   // Drawing code
									   //set to wireframe so we can see the triangles
									   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

									   // Draw the scene
									   //need to 'use' the shaders before updating uniforms
		glUseProgram(shaderProgram);

		//set background image
		modelLocation = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
		viewLocation = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		projectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[3].texture);
		background.render();

		//set paddle image
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(paddleTranslate*mScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[16].texture);
		paddle.render();

		//reset translation matrix
		mTranslate = glm::mat4(1.0f);

		//translate to the top left of the squares
		mTranslate = glm::translate(mTranslate, glm::vec3(-0.8f, 0.9f, 0.0f));

		//position and draw the squares
		for (int x = 0; x < 11; x++)
		{
			//loads in the texture assigned to that row
			//row finishes and moves onto next texture
			glBindTexture(GL_TEXTURE_2D, texArray[x + 5].texture);

			for (int y = 0; y < 9; y++)
			{
				modelMatrix = mTranslate * mRotate * mScale;
				//transform uniforms
				//set current value of transform matrix in shader
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

				//gap inbetween blocks on x axis
				mTranslate = glm::translate(mTranslate, glm::vec3(0.2f, 0.0f, 0.0f));
				//bind the texture we want to use				
				//glBindTexture(GL_TEXTURE_2D, texArray[y + 5].texture);
				//Draw the grid - call its render method
				squares[x][y].render();
			}

			mTranslate = glm::mat4(1.0f);
			//set start of next line of blocks
			mTranslate = glm::translate(mTranslate, glm::vec3(-0.8f, 0.9f - ((x + 1) / 12.0f), 0.0f));

		}
		//reset the translation matrix using the 'identity' matrix
		mTranslate = glm::mat4(1.0f);

		SDL_GL_SwapWindow(win);


		//Process input
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				windowOpen = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					paddleTranslate = glm::translate(paddleTranslate, glm::vec3(-0.05f, 0.0f, 0.0f));
					break;
				case SDLK_RIGHT:
					paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.05f, 0.0f, 0.0f));
					break;
				case SDLK_UP:
					paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, 0.05f, 0.0f));
					break;
				case SDLK_DOWN:
					paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, -0.05f, 0.0f));
					break;
				case SDLK_q:
					viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.01f, 0.0f, 0.0f));
					break;
				case SDLK_e:
					viewMatrix = glm::translate(viewMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
					break;
					//closes the program via key
				case SDLK_ESCAPE:
					windowOpen = false;
					break;
				default:
					break;
				}
			}
		}
	}


	// Clean up
	SDL_Log("Finished. Cleaning up and closing down\n");

	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);
	delete[] texArray;

	SDL_Quit();
	return 0;
}


#endif