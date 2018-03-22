#include <iostream>
#include "Circle.h"
#include "Square.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include "Boundary.h"
#include "Globals.h"
#include "TextSquare.h"
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


bool checkCollision(float aX, float aY, float aW, float aH, float bX, float bY, float bW, float bH);
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
	win = SDL_CreateWindow("Reece Goodwin, CGP2012M, GOO15576606", 100, 100, 800, 600,
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
	Square bricks[11][9];
	Square background;
	Square midground;
	Square paddle;
	Square ball;
	Square lives[3];
	Boundary boundaryTop(0);
	Boundary boundaryLeft(1);
	Boundary boundaryRight(2);

	//create textures
	texArray = new Texture[17];

	//background textures
	texArray[0].load("..//content//sky.png");
	texArray[0].setBuffers();
	texArray[1].load("..//content//xpgrass.png");
	texArray[1].setBuffers();
	
	//loading in the block textures
	texArray[2].load("..//content//Green Block.png");
	texArray[2].setBuffers();
	texArray[3].load("..//content//Red Block.png");
	texArray[3].setBuffers();
	texArray[4].load("..//content//Purple Block.png");
	texArray[4].setBuffers();
	texArray[5].load("..//content//Yellow Block.png");
	texArray[5].setBuffers();
	texArray[6].load("..//content//Blue Block.png");
	texArray[6].setBuffers();
	texArray[7].load("..//content//Misty Blue Block.png");
	texArray[7].setBuffers();
	texArray[8].load("..//content//Orange Block.png");
	texArray[8].setBuffers();
	texArray[9].load("..//content//Grey Block.png");
	texArray[9].setBuffers();
	texArray[10].load("..//content//Dark Green Block.png");
	texArray[10].setBuffers();
	texArray[11].load("..//content//Black Block.png");
	texArray[11].setBuffers();
	texArray[12].load("..//content//Cyan Block.png");
	texArray[12].setBuffers();

	//player paddle
	texArray[13].load("..//content//Paddle.png");
	texArray[13].setBuffers();
	//ball
	texArray[14].load("..//content//Ball.png");
	texArray[14].setBuffers();
	//lives
	texArray[15].load("..//content//Life.png");
	texArray[15].setBuffers();
	//boundary
	texArray[16].load("..//content//Boundary.png");
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
	
	//for the grid of square blocks
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			bricks[i][j].setBuffers();
		}
	}

	//for the lives in a row
	for (int i = 0; i < 3; i++)
	{
		lives[i].setBuffers();
	}

	background.setBuffers();
	midground.setBuffers();
	paddle.setBuffers();
	ball.setBuffers();
	boundaryLeft.setBuffers();
	boundaryRight.setBuffers();
	boundaryTop.setBuffers();

	///tranform matrices
	//Matrix
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	//Translate
	glm::mat4 mTranslate;
	glm::mat4 backgroundTranslate;
	glm::mat4 midgroundTranslate;
	glm::mat4 paddleTranslate;
	glm::mat4 ballTranslate;
	//Rotate
	glm::mat4 mRotate;
	//Scale
	glm::mat4 backgroundScale;
	glm::mat4 midgroundScale;
	glm::vec3 scaleFactor;
	glm::vec3 b_scaleFactor;
	glm::mat4 ballScale;
	glm::mat4 lifeScale;
	glm::mat4 mScale;
	glm::mat4 boundaryScale;
	glm::mat4 paddleScale;

	//once only scale to square
	scaleFactor = { 1.0f, 0.4f, 1.0f };
	mScale = glm::scale(mScale, glm::vec3(scaleFactor));

	//once only scale to background
	b_scaleFactor = { 20.0f, 10.0f, 1.0f };
	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, 0.0f));
	
	//scale midground
	midgroundScale = glm::scale(midgroundScale, glm::vec3(b_scaleFactor));
	midgroundTranslate = glm::translate(midgroundTranslate, glm::vec3(0.0f, 0.0f, 0.0f));

	//set initial position of paddle and scaling to fit
	paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, -0.95f, 0.0f));
	paddleScale = glm::scale(paddleScale, glm::vec3(1.5f, 0.2f, 0.0f));

	//set the initial position of the ball
	ballTranslate = glm::translate(ballTranslate, glm::vec3(0.01f, -0.9f, 0.0f));
	ballScale = glm::scale(ballScale, glm::vec3(0.3f, 0.3f, 0.0f));

	//setting up the lives
	//scales the size by half the image size
	lifeScale = glm::scale(lifeScale, glm::vec3(0.5f, 0.5f, 0.0f));

	//set the boundaries
	
	boundaryScale = glm::scale(boundaryScale, glm::vec3(1.0f, 1.0f, 0.0f));

	//set view matrix to identity
	//matrix equivalent of '1'
	viewMatrix = glm::mat4(1.0f);

	//orthographic projection
	//left, right, bottom, top, near clip plane, far clip plane
	//set width and height values
	projectionMatrix = glm::ortho(-1.0f*aspectRatio, 1.0f*aspectRatio, -1.0f, 1.0f, -1.0f, 100.0f);

	//A game loop, rendering, then handling input
	SDL_Event event;

	//variables
	Uint32 flags;
	float ballX, ballY;
	int livesLeft = 3;


	//bools
	bool windowOpen = true;
	bool ballMoving = false;
	bool ballLeft, ballDown = false;

	///////////////////
	///THE GAME LOOP///
	///////////////////
	while (windowOpen)
	{
		//updates the window size, allows for resizing and maintains aspect ratio
		int w;
		int h;
		SDL_GetWindowSize(win, &w, &h);

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
		glBindTexture(GL_TEXTURE_2D, texArray[0].texture);
		background.render();

		//set midground image
		modelLocation = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(midgroundTranslate*midgroundScale));
		viewLocation = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		projectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[1].texture);
		midground.render();


		//set paddle image
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(paddleTranslate*paddleScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[13].texture);
		paddle.render();

		//set ball image
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(ballTranslate * ballScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[14].texture);
		ball.render();

		//setting boundary images
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mTranslate * boundaryScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[16].texture);
		boundaryTop.render();
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mTranslate * boundaryScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[16].texture);
		boundaryLeft.render();
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mTranslate * boundaryScale));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[16].texture);
		boundaryRight.render();

		//reset translation matrix
		mTranslate = glm::mat4(1.0f);

		//translate to the top left of the squares
		mTranslate = glm::translate(mTranslate, glm::vec3(-0.77f, 0.8f, 0.0f));

		//position and draw the squares
		for (int x = 0; x < 11; x++)
		{
			//loads in the texture assigned to that row
			//row finishes and moves onto next texture
			glBindTexture(GL_TEXTURE_2D, texArray[x + 2].texture);

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
				//Draw the grid - call its render method
				bricks[x][y].render();
			}

			mTranslate = glm::mat4(1.0f);
			//set start of next line of blocks
			mTranslate = glm::translate(mTranslate, glm::vec3(-0.77f, 0.8f - ((x + 1) / 11.0f), 0.0f));
		}

		//reset
		mTranslate = glm::mat4(1.0f);

		//translate the first life
		mTranslate = glm::translate(mTranslate, glm::vec3(0.75f, 0.9f, 0.0f));

		//position and draw the lives
		for (int x = 0; x < 3; x++)
		{
			modelMatrix = mTranslate * mRotate * lifeScale;

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glBindTexture(GL_TEXTURE_2D, texArray[15].texture);

			mTranslate = glm::translate(mTranslate, glm::vec3(0.1f, 0.0f, 0.0f));
			if (x < livesLeft)
				lives[x].render();
		}

		//reset the translation matrix using the 'identity' matrix
		mTranslate = glm::mat4(1.0f);

		SDL_GL_SwapWindow(win);

		//Process inputs
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT)
			{
				glViewport(0, 0, w, h);
			}
			if (event.type == SDL_QUIT)
			{
				windowOpen = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					//player character movement
					case SDLK_LEFT:

					if (paddleTranslate[3].x <= -0.8f)
					{
						paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, 0.0f, 0.0f));
					}
					else
					{
						paddleTranslate = glm::translate(paddleTranslate, glm::vec3(-0.05f, 0.0f, 0.0f));
						backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.01f, 0.0f, 0.0f));
						midgroundTranslate = glm::translate(midgroundTranslate, glm::vec3(0.005f, 0.0f, 0.0f));
						if (ballMoving == false) //stays with the paddle until shot
						{
							ballTranslate = glm::translate(ballTranslate, glm::vec3(-0.05f, 0.0f, 0.0f));
						}
					}

					break;
					case SDLK_RIGHT:

					if (paddleTranslate[3].x >= 0.85f)
					{
						paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.0f, 0.0f, 0.0f));
					}
					else
					{
						paddleTranslate = glm::translate(paddleTranslate, glm::vec3(0.05f, 0.0f, 0.0f));
						backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(-0.01f, 0.0f, 0.0f));
						midgroundTranslate = glm::translate(midgroundTranslate, glm::vec3(-0.005f, 0.0f, 0.0f));
						if (ballMoving == false)
						{
							ballTranslate = glm::translate(ballTranslate, glm::vec3(0.05f, 0.0f, 0.0f));
						}
					}
					
					break;

				case SDLK_UP:
					if (ballMoving == false)
						ballMoving = true; //shoots ball
					break;

					//camera movement
				case SDLK_q:
					viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.01f, 0.0f, 0.0f));

					break;
				case SDLK_e:
					viewMatrix = glm::translate(viewMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
					break;

					//fullscreen mode
				case SDLK_f:
					flags = SDL_GetWindowFlags(win);
					if (flags & SDL_WINDOW_FULLSCREEN)
					{
						SDL_SetWindowFullscreen(win, 0);
					}
					else
					{
						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
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
		//ball is moving right
		if (ballTranslate[3].x >= 0.97f)
		{
			ballLeft = false;
		}
		//ball is moving left
		if (ballTranslate[3].x <= -0.92f)
		{
			ballLeft = true;
		}
		//ball is moving up
		if (ballTranslate[3].y >= 0.93f)
		{
			ballDown = true;
		}
		//ball is moving down
		if (ballTranslate[3].y <= -1.0f)
		{
			SDL_Log("YOU DIED");
			ballDown = false;

			//loses 1 life
			livesLeft--;
		}

		if (ballMoving == true)
		{
			ballX = 0.008f;
			ballY = 0.008f;

			if (!ballLeft)
				ballX = -ballX;
			if (ballDown)
				ballY = -ballY;

			ballTranslate = glm::translate(ballTranslate, glm::vec3(ballX, ballY, 0.0f));
		}

		if (checkCollision(ballTranslate[3].x, ballTranslate[3].y, ballTranslate[3].x + 0.5f, ballTranslate[3].y + 0.5f, paddleTranslate[3].x, paddleTranslate[3].y, paddleTranslate[3].x + 0.5f, paddleTranslate[3].y + 0.5f))
		{
			SDL_Log("Collision Detected!!! WE HAVE LIFE");
		}

		if (livesLeft == 0)
		{
			SDL_Log("Game Over!");
			SDL_Delay(1000);
			break;
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

///////////////
///Collision///
///////////////
bool checkCollision(float aX, float aY, float aW, float aH, float bX, float bY, float bW, float bH)
{
	if (aY + aH < bY) return false;
	else if (aY > bY + bH) return false;
	else if (aX + aW < bX) return false;
	else if (aX > bX + bW) return false;

	return true;
}


#endif