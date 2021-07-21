#include <iostream>
#include <sstream>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h"	// Important - this header must come before glfw3 header
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"

//-----------------------------------------------------------------------------		//
// GLOBAL VARIABLES																	//
//-----------------------------------------------------------------------------  	//
const char* APP_TITLE = "Trabalho Final CG - OPENGL";								//
int gWindowWidth = 1280;															//
int gWindowHeight = 800;															//
GLFWwindow* gWindow = NULL;															//
bool gWireframe = false;															//
																					//
//-----------------------------------------------------------------------------		//
// Lights																			//
//-----------------------------------------------------------------------------		//
bool gFlashlightOn = false;															//
																					//
bool gPost0lightOn = false;															//
bool gPost1lightOn = false;															//
bool gPost2lightOn = false;															//
bool gPost3lightOn = false;															//
																					//
bool gSunLightOn = false;															//
																					//
glm::vec4 gClearColor(0.06f, 0.06f, 0.06f, 1.0f);									//
																					//
																					//
//-----------------------------------------------------------------------------		//
// Camera																			//
//-----------------------------------------------------------------------------		//
FPSCamera fpsCamera(glm::vec3(0.0f, 8.0f, 10.0f));									//
const double ZOOM_SENSITIVITY = -3.0;												//
const float MOVE_SPEED = 8.0; // speed												//
const float MOUSE_SENSITIVITY = 0.1f;												//
																					//
																					//
// Function prototypes																//
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);	//
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);				//
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);			//
void update(double elapsedTime);													//
void showFPS(GLFWwindow* window);													//
bool initOpenGL();																	//

int rotate_flag = 1;
bool wall_flag = true;
bool lousa_flag = false;
bool toy_flag = false;
bool view_toy = false;
bool test_flag = false;

//-----------------------------------------------------------------------------
// Main Application Entry Point
//-----------------------------------------------------------------------------
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// 1. Set up an array of vectex data for a cube with index buffer data
	GLfloat vertices[] = {
		// position		 // tex coords

	   // front face
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
	   -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
	   -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		// back face
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		 // left face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

		 // right face
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

		  // top face
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
		  1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		 -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
		  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

		  // bottom face
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		  1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
		 -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
	};

	// Cube position
	glm::vec3 cubePos = glm::vec3(0.0f, 10.0f, 0.0f);

	// 2. Set up buffers on the GPU
	GLuint vbo, vao;

	glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// copy the data from CPU to GPU

	glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(vao);					// Make it the current one

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	// Texture Coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// --------------------------------------------------- //
	// Structured Program                                  //
	// --------------------------------------------------- //


	ShaderProgram lightingShader;
	ShaderProgram simpleDepthShader;

	lightingShader.loadShaders("shaders/lighting_dir_point_spot.vert", "shaders/lighting_dir_point_spot.frag");
	simpleDepthShader.loadShaders("shaders/lighting_dir_point_spot_depth.vert", "shaders/lighting_dir_point_spot_depth.frag");

	// Load meshes and textures
	const int numModels = 45;
	Mesh mesh[numModels];
	Texture2D texture[numModels];

	mesh[0].loadOBJ("objetos/armario_secretario_d/armario_secretario1.obj");
	mesh[1].loadOBJ("objetos/armario_secretario_d/armario_secretario2.obj");
	mesh[2].loadOBJ("objetos/armario_vertical_d/armario_vertical1.obj");
	mesh[3].loadOBJ("objetos/armarios_cima_d/armarios_cima1.obj");
	mesh[4].loadOBJ("objetos/armarios_cima_d/armarios_cima2.obj");
	mesh[5].loadOBJ("objetos/armarios_gerais_d/armarios_gerais1.obj");
	mesh[6].loadOBJ("objetos/armarios_pessoais_d/armarios_pessoais1.obj");
	mesh[7].loadOBJ("objetos/armarios_pessoais_d/armarios_pessoais2.obj");
	mesh[8].loadOBJ("objetos/cadeiras_d/cadeira1.obj");
	mesh[9].loadOBJ("objetos/cadeiras_d/cadeira2.obj");
	mesh[10].loadOBJ("objetos/cadeiras_d/cadeira3.obj");
	mesh[11].loadOBJ("objetos/cadeiras_d/cadeira4.obj");
	mesh[12].loadOBJ("objetos/cadeiras_d/cadeira5.obj");
	mesh[13].loadOBJ("objetos/cadeiras_d/cadeira6.obj");
	mesh[14].loadOBJ("objetos/camera_d/camera1.obj");
	mesh[15].loadOBJ("objetos/CPU_d/cpu1.obj");
	mesh[16].loadOBJ("objetos/CPU_d/cpu2.obj");
	mesh[17].loadOBJ("objetos/CPU_d/cpu3.obj");
	mesh[18].loadOBJ("objetos/CPU_d/cpu4.obj");
	mesh[19].loadOBJ("objetos/estande_d/estande1.obj");
	mesh[20].loadOBJ("objetos/impressora_d/impressora1.obj");
	mesh[21].loadOBJ("objetos/janela_d/janela1.obj");
	mesh[22].loadOBJ("objetos/janela_d/janela2.obj");
	mesh[23].loadOBJ("objetos/janela_d/janela3.obj");
	mesh[24].loadOBJ("objetos/estrutura_d/lousa-simples.obj");
	mesh[25].loadOBJ("objetos/mesa_d/mesa_total1.obj");
	mesh[26].loadOBJ("objetos/monitor_d/monitor1.obj");
	mesh[27].loadOBJ("objetos/monitor_d/monitor2.obj");
	mesh[28].loadOBJ("objetos/monitor_d/monitor3.obj");
	mesh[29].loadOBJ("objetos/monitor_d/monitor4.obj");
	mesh[30].loadOBJ("objetos/notebook_d/notebook1.obj");
	mesh[31].loadOBJ("objetos/notebook_d/notebook2.obj");
	mesh[32].loadOBJ("objetos/porta_d/porta1.obj");
	mesh[33].loadOBJ("objetos/projetor_d/projetor1.obj");
	mesh[34].loadOBJ("objetos/sofa/sofa1.obj");
	mesh[35].loadOBJ("models/floor.obj");
	mesh[36].loadOBJ("objetos/estrutura_d/parede1.obj");
	mesh[37].loadOBJ("objetos/estrutura_d/parede2.obj");
	mesh[38].loadOBJ("objetos/estrutura_d/parede3.obj");
	mesh[39].loadOBJ("objetos/estrutura_d/parede4.obj");
	mesh[40].loadOBJ("objetos/handles_d/handles2.obj");
	mesh[41].loadOBJ("objetos/handles_d/handles3.obj");

	texture[0].loadTexture("textures/branco.png", true);
	texture[1].loadTexture("textures/cadeira.jpg", true);
	texture[2].loadTexture("textures/cinza.jpg", true);
	texture[3].loadTexture("textures/impressora.jpg", true);
	texture[4].loadTexture("textures/vermelho.jpg", true);
	texture[5].loadTexture("textures/roxo.jpg", true);
	texture[6].loadTexture("textures/patrick_tex.png", true);
	texture[7].loadTexture("textures/cg1.png", true);
	texture[8].loadTexture("textures/bege.jpg", true);
	texture[9].loadTexture("textures/chao.jpg", true);
	texture[10].loadTexture("textures/marrom.jpg", true);
	texture[11].loadTexture("textures/wood_floor.jpg", true);
	texture[12].loadTexture("textures/cg2.png", true);

	// Model positions
	glm::vec3 modelPos[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),	// Pos 0 - armario_secretario
		glm::vec3(7.0f, 0.0f, -10.0f),	// Pos 1 - cadeira 1
		glm::vec3(7.0f, 0.0f, -21.5f),	// Pos 2 - cadeira 2
		glm::vec3(0.0f, 0.0f, 0.0f),	// Pos 3 - cadeira 3
		glm::vec3(14.0f, 0.0f, 2.0f),	// Pos 4 - cadeira 4
		glm::vec3(-10.0f, 0.0f, 0.0f),	// Pos 5 - janela 2
		glm::vec3(10.0f, 0.0f, 0.0f),	// Pos 6 - janela 3
		glm::vec3(6.5f, 0.0f, -12.0f),	// Pos 7 - mesa 2
		glm::vec3(0.0f, 0.0f, -0.5f),	// Pos 8 -
		glm::vec3(-37.0f, 0.0f, 15.5f),	// Pos 9 -
		glm::vec3(40.0f, 0.0f, 0.0f),   // Pos 10 -
	};

	// Model scale
	glm::vec3 modelScale[] = {
		glm::vec3(1.0f, 1.0f, 1.0f),	// Sca 0
		glm::vec3(1.0f, 1.0f, 1.0f),	// Sca 1
		glm::vec3(5.0f, 5.0f, 5.0f),	// Sca 2
		glm::vec3(1.0f, 1.0f, 1.0f),	// Sca 3
		glm::vec3(0.1f, 0.1f, 0.1f),	// Sca 4
		glm::vec3(0.7f, 0.7f, 0.7f),	// Sca 5
		glm::vec3(1.0f, 1.0f, 1.0f),	// Sca 6 
		glm::vec3(1.04f, 1.0f, 1.0f),	// Sca 7 - mesa 1
		glm::vec3(0.58f, 1.0f, 1.0f),	// Sca 8 - mesa 2
		glm::vec3(0.1f, 0.1f, 0.1f),	// Sca
		glm::vec3(10.0f, 10.0f, 10.0f),
	};

	// Point Light positions
	glm::vec3 pointLightPos[4] = {
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(-3.5f, 15.0f, -1.0f)
	};


	double lastTime = glfwGetTime();

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lightingShader.use();
	lightingShader.setUniform("diffuseTexture", 0);
	lightingShader.setUniform("shadowMap", 1);
	

	float chairAngle = 0.0f;
	float toyAngle = 0.0f;
	float lousaAngle = 0.0f;

	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;

		// Poll for and process events
		glfwPollEvents();
		update(deltaTime);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0), view(1.0), projection(1.0);

		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightProjection, lightView;

		// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

		// update the view (camera) position
		glm::vec3 viewPos;
		viewPos.x = fpsCamera.getPosition().x;
		viewPos.y = fpsCamera.getPosition().y;
		viewPos.z = fpsCamera.getPosition().z;

		glm::mat4 modelcubo(1.0);
		modelcubo = glm::translate(modelcubo, cubePos) * glm::scale(glm::mat4(1.0), glm::vec3(10.0f, 0.01f, 10.0f));

		// Must be called BEFORE setting uniforms because setting uniforms is done on the currently active shader program.
		lightingShader.use();
		lightingShader.setUniform("model", glm::mat4(1.0));  
		lightingShader.setUniform("view", view);
		lightingShader.setUniform("projection", projection);
		lightingShader.setUniform("viewPos", viewPos);

		// Directional light
		lightingShader.setUniform("sunLight.direction", glm::vec3(0.0f, -0.9f, -0.17f));
		lightingShader.setUniform("sunLight.ambient", glm::vec3(0.7f, 0.7f, 0.7f));
		lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));		// dark
		lightingShader.setUniform("sunLight.specular", glm::vec3(0.7f, 0.7f, 0.7f));
		lightingShader.setUniform("sunLight.on", gSunLightOn);

		// Point Light 1
		lightingShader.setUniform("pointLights[0].ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("pointLights[0].diffuse", glm::vec3(0.0f, 8.0f, 0.1f));	// green-ish light
		lightingShader.setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[0].position", pointLightPos[0]);
		lightingShader.setUniform("pointLights[0].constant", 1.0f);
		lightingShader.setUniform("pointLights[0].linear", 0.22f);
		lightingShader.setUniform("pointLights[0].exponent", 0.01f);
		lightingShader.setUniform("pointLights[0].on", gPost0lightOn);

		// Point Light 2
		lightingShader.setUniform("pointLights[1].ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("pointLights[1].diffuse", glm::vec3(8.0f, 0.1f, 0.0f));	// red-ish light
		lightingShader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[1].position", pointLightPos[1]);
		lightingShader.setUniform("pointLights[1].constant", 1.0f);
		lightingShader.setUniform("pointLights[1].linear", 0.22f);
		lightingShader.setUniform("pointLights[1].exponent", 0.01f);
		lightingShader.setUniform("pointLights[1].on", gPost1lightOn);

		// Point Light 3
		lightingShader.setUniform("pointLights[2].ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("pointLights[2].diffuse", glm::vec3(0.0f, 0.1f, 8.0f));	// blue-ish light
		lightingShader.setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[2].position", pointLightPos[2]);
		lightingShader.setUniform("pointLights[2].constant", 1.0f);
		lightingShader.setUniform("pointLights[2].linear", 0.22f);
		lightingShader.setUniform("pointLights[2].exponent", 0.01f);
		lightingShader.setUniform("pointLights[2].on", gPost2lightOn);

		// Point Light 4
		lightingShader.setUniform("pointLights[3].ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("pointLights[3].diffuse", glm::vec3(8.0f, 8.0f, 8.0f));//sin(currentTime) * 50.0f, cos(currentTime) * 33.0f, sin(currentTime) * 90.0f));	// rainbow light
		lightingShader.setUniform("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("pointLights[3].position", pointLightPos[3]);
		lightingShader.setUniform("pointLights[3].constant", 1.0f);
		lightingShader.setUniform("pointLights[3].linear", 0.02f);
		lightingShader.setUniform("pointLights[3].exponent", 0.01f);
		lightingShader.setUniform("pointLights[3].on", gPost3lightOn);

		// Spot light
		glm::vec3 spotlightPos = fpsCamera.getPosition();

		// offset the flash light down a little
		spotlightPos.y -= 0.5f;

		lightingShader.setUniform("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.setUniform("spotLight.position", spotlightPos);
		lightingShader.setUniform("spotLight.direction", fpsCamera.getLook());
		lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(15.0f)));
		lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(20.0f)));
		lightingShader.setUniform("spotLight.constant", 5.0f);
		lightingShader.setUniform("spotLight.linear", 0.07f);
		lightingShader.setUniform("spotLight.exponent", 0.017f);
		lightingShader.setUniform("spotLight.on", gFlashlightOn);



		/*simpleDepthShader.use();
		simpleDepthShader.setUniform("lightSpaceMatrix", lightSpaceMatrix);*/


		// Render the scene
		//for (int i = 0; i < numModels; i++)
		//{
		//	model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
		//	lightingShader.setUniform("model", model);

		//	// Set material properties
		//	lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		//	lightingShader.setUniformSampler("material.diffuseMap", 0);
		//	lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		//	lightingShader.setUniform("material.shininess", 32.0f);

		//	texture[i].bind(0);		
		//	mesh[i].draw();			
		//	texture[i].unbind(0);

		//}

		//lightingShader.setUniform("modelcubo", modelcubo);

		//glBindVertexArray(vao);

		//// Draw the crate
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		// ------------------------------------------------------------------------------------------------- //
		// OBJ 0 - Armario_Secretario Esquerda
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modela1(1.0);
		modela1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modela1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[0].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 0 - Armario_Secretario Direita
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modela2(1.0);
		modela2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modela2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[1].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 1 - Armario Vertical
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelv1(1.0);
		modelv1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelv1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[2].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 2 - Armario Cima Esquerda
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelac1(1.0);
		modelac1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelac1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[3].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 2 - Armario Cima Direita
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelac2(1.0);
		modelac2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelac2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[4].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 3 - Armarios Gerais
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelag1(1.0);
		modelag1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelag1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 64.0f);

		texture[0].bind(0);
		mesh[5].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 4 - Armarios Pessoais Esquerda
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelap1(1.0);
		modelap1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelap1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[6].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 4 - Armarios Pessoais Direita
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelap2(1.0);
		modelap2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelap2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[7].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 1
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca1(1.0);
		modelca1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[8].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 2
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca2(1.0);
		modelca2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[9].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 3
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca3(1.0);
		modelca3 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca3);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[10].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 4
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca4(1.0);
		modelca4 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca4);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[11].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 5
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca5(1.0);
		modelca5 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca5);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[12].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 5 - Cadeira 6
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelca6(1.0);
		modelca6 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelca6);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[1].bind(0);
		mesh[13].draw();
		texture[1].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 6 - Camera
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelcam(1.0);
		modelcam = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelcam);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[4].bind(0);
		mesh[14].draw();
		texture[4].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 7 - CPU 1
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelcp1(1.0);
		modelcp1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelcp1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[15].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 7 - CPU 2
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelcp2(1.0);
		modelcp2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelcp2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[16].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 7 - CPU 3
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelcp3(1.0);
		modelcp3 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelcp3);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[17].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 7 - CPU 4
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelcp4(1.0);
		modelcp4 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelcp4);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[18].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 8 - Estande
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modeles(1.0);
		modeles = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modeles);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[19].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 9 - Impressora
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelim(1.0);
		modelim = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelim);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[3].bind(0);
		mesh[20].draw();
		texture[3].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 10 - Janela Esquerda
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelja1(1.0);
		modelja1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelja1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[5].bind(0);
		mesh[21].draw();
		texture[5].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 10 - Janela Centro
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelja2(1.0);
		modelja2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelja2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[5].bind(0);
		mesh[22].draw();
		texture[5].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 10 - Janela Direita
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelja3(1.0);
		modelja3 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelja3);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[5].bind(0);
		mesh[23].draw();
		texture[5].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 11 - Lousa
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modello(1.0);
		
		lousaAngle += (float)(deltaTime * 50.0f);
		if (lousaAngle >= 360.0f) lousaAngle = 0.0f;
		
		modello = glm::translate(glm::mat4(1.0), modelPos[9]) * glm::rotate(glm::mat4(1.0), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modello);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		if (lousaAngle <= 180.0f) {
			texture[7].bind(0);
			mesh[24].draw();
			texture[7].unbind(0);
		}

		else {
			texture[12].bind(0);
			mesh[24].draw();
			texture[12].unbind(0);
		}

		/*if (lousa_flag) {
			texture[7].bind(0);
			mesh[24].draw();
			texture[7].unbind(0);
		}

		else {
			texture[12].bind(0);
			mesh[24].draw();
			texture[12].unbind(0);
		}*/
		// ------------------------------------------------------------------------------------------------- //
		// OBJ 12 - Mesa
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelme(1.0);
		modelme = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelme);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[25].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 13 - Monitor 1
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelmo1(1.0);
		modelmo1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelmo1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[26].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 13 - Monitor 2
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelmo2(1.0);
		modelmo2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelmo2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[27].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 13 - Monitor 3
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelmo3(1.0);
		modelmo3 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelmo3);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[28].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 13 - Monitor 4
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelmo4(1.0);
		modelmo4 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelmo4);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[29].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 14 - Notebook 1
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelnb1(1.0);
		modelnb1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelnb1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[30].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 14 - Notebook 2
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelnb2(1.0);
		modelnb2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelnb2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[31].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 15 - Porta
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelpo(1.0);
		modelpo = glm::translate(glm::mat4(1.0), modelPos[8]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelpo);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[8].bind(0);
		mesh[32].draw();
		texture[8].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 16 - Projetor
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelpr(1.0);
		modelpr = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelpr);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[33].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 17 - Sofa
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelso(1.0);
		modelso = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelso);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[10].bind(0);
		mesh[34].draw();
		texture[10].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 18 - Chao
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelch(1.0);
		modelch = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[2]);
		lightingShader.setUniform("model", modelch);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);
		
		if (wall_flag) {
			texture[9].bind(0);
			mesh[35].draw();
			texture[9].unbind(0);
		}

		else {
			texture[11].bind(0);
			mesh[35].draw();
			texture[11].unbind(0);
		}

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 19 - Parede Esquerda
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelp1(1.0);
		modelp1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelp1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[36].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 19 - Parede Frente
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelp2(1.0);
		modelp2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelp2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[37].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 19 - Parede Direita
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelp3(1.0);
		modelp3 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelp3);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[4].bind(0);
		mesh[38].draw();
		texture[4].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 19 - Parede Tras
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelp4(1.0);
		modelp4 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelp4);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[0].bind(0);
		mesh[39].draw();
		texture[0].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 20 - Puxadores 1
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelpu1(1.0);
		modelpu1 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelpu1);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[40].draw();
		texture[2].unbind(0);

		// ------------------------------------------------------------------------------------------------- //
		// OBJ 20 - Puxadores 2
		// ------------------------------------------------------------------------------------------------- //

		glm::mat4 modelpu2(1.0);
		modelpu2 = glm::translate(glm::mat4(1.0), modelPos[0]) * glm::scale(glm::mat4(1.0), modelScale[0]);
		lightingShader.setUniform("model", modelpu2);

		lightingShader.setUniform("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniformSampler("material.diffuseMap", 0);
		lightingShader.setUniform("material.specular", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setUniform("material.shininess", 32.0f);

		texture[2].bind(0);
		mesh[41].draw();
		texture[2].unbind(0);		

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

				// Swap front and back buffers
		glfwSwapBuffers(gWindow);

		lastTime = currentTime;
	}

	glfwTerminate();

	return 0;
}

//-----------------------------------------------------------------------------
// Initialize GLFW and OpenGL
//-----------------------------------------------------------------------------
bool initOpenGL()
{
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3


	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

	// Hides and grabs cursor, unlimited movement
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	glClearColor(gClearColor.r, gClearColor.g, gClearColor.b, gClearColor.a);

	// Define the viewport dimensions
	glViewport(0, 0, gWindowWidth, gWindowHeight);
	glEnable(GL_DEPTH_TEST);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		gWireframe = !gWireframe;
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		// toggle the flashlight
		gFlashlightOn = !gFlashlightOn;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		// toggle the pointlight
		gPost0lightOn = !gPost0lightOn;
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		// toggle the pointlight
		gPost1lightOn = !gPost1lightOn;
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		// toggle the pointlight
		gPost2lightOn = !gPost2lightOn;
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		// toggle the pointlight
		gPost3lightOn = !gPost3lightOn;
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		// toggle the sunlight
		gSunLightOn = !gSunLightOn;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{

		wall_flag = !wall_flag;
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{

		toy_flag = !toy_flag;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{

		view_toy = !view_toy;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{

		lousa_flag = !lousa_flag;
	}

}

//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	gWindowWidth = width;
	gWindowHeight = height;
	glViewport(0, 0, gWindowWidth, gWindowHeight);
}

//-----------------------------------------------------------------------------
// Called by GLFW when the mouse wheel is rotated
//-----------------------------------------------------------------------------
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
	double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

	fov = glm::clamp(fov, 1.0, 120.0);

	fpsCamera.setFOV((float)fov);
}

//-----------------------------------------------------------------------------
// Update stuff every frame
//-----------------------------------------------------------------------------
void update(double elapsedTime)
{
	// Camera orientation
	double mouseX, mouseY;

	// Get the current mouse cursor position delta
	glfwGetCursorPos(gWindow, &mouseX, &mouseY);

	// Rotate the camera the difference in mouse distance from the center screen.  Multiply this delta by a speed scaler
	fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);

	// Clamp mouse cursor to center of screen
	glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

	// Camera FPS movement

	// Forward/backward
	if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
	else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

	// Strafe left/right
	if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
	else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

	// Up/down
	if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * glm::vec3(0.0f, 1.0f, 0.0f));
	else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
		fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -glm::vec3(0.0f, 1.0f, 0.0f));
}

//-----------------------------------------------------------------------------
// Code computes the average frames per second.
//-----------------------------------------------------------------------------
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); 

	elapsedSeconds = currentSeconds - previousSeconds;

	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(2);	
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}

	frameCount++;
}
