/*---------------------------------------------------------*/
/* ----------------  PrÃ¡ctica                   -----------*/
/*-----------------    2022-2   ---------------------------*/
/*------------- Alumno:                     ---------------*/
/*------------- No. Cuenta                  ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <iostream>
#include "include\irrKlang-1.6.0/irrKlang.h" //Audio Library

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//Starts the sound engine
irrklang::ISoundEngine* Background = irrklang::createIrrKlangDevice();


//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

//Audio


// camera
Camera camera(glm::vec3(0.0f, 50.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f,
		orientaArbolX = 0.0f,
		orientaArbolY = 0.0f,
		orientaHojaX = 0.0f,
		orientaHojaY = 0.0f,
		trasladaHojaY = 0.0f,
		trasladaHojaZ = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false,
		recorrido7 = false,
		recorrido8 = false,
		recorrido9 = false,
		estadoArbol1 = true,
		estadoArbol2 = false,
		estadoHoja1 = false,
		estadoHoja2 = true,
		estadoHoja3 = false,
		estadoHoja4 = false,
		estadoHoja5 = false,
		estadoHoja6 = false,
		estadoHoja7 = false,
		estadoHoja8 = false,
		estadoHoja9 = false,
		estadoHoja10 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//VehÃ­culo
	if (animacion)
	{
		if (recorrido1) {
			movAuto_x -= 10.0f;
			if (movAuto_x <= -500.0f) {
				recorrido1 = false;
				recorrido2 = true;
				movAuto_x = -500.0f;
			}
		}
		if (recorrido2) {
			movAuto_x -= 5.0f;
			movAuto_z += 5.0f;
			orienta = 50.0f;
			if (movAuto_x <= -800.0f) {
				recorrido2 = false;
				recorrido3 = true;
				movAuto_x = -800.0f;
			}
		}
		if (recorrido3) {
			movAuto_z += 10.0f;
			orienta = 90.0f;
			if (movAuto_z >= 1800.0f) {
				recorrido3 = false;
				recorrido4 = true;
				movAuto_z = 1800.0f;
			}
		}
		if (recorrido4) {
			movAuto_x += 5.0f;
			movAuto_z += 5.0f;
			orienta = 140.0f;
			if (movAuto_x >= -500.0f) {
				recorrido4 = false;
				recorrido5 = true;
				movAuto_x = -500.0f;
			}
		}
		if (recorrido5) {
			movAuto_x += 10.0f;
			orienta = 180.0f;
			if (movAuto_x >= 500.0f) {
				recorrido5 = false;
				recorrido6 = true;
				movAuto_x = 500.0f;
			}
		}
		if (recorrido6) {
			movAuto_x += 5.0f;
			movAuto_z -= 5.0f;
			orienta = 230.0f;
			if (movAuto_x >= 800.0f) {
				recorrido6 = false;
				recorrido7 = true;
				movAuto_x = 800.0f;
			}
		}
		if (recorrido7) {
			movAuto_z -= 10.0f;
			orienta = 270.0f;
			if (movAuto_z <= 230.0f) {
				recorrido7 = false;
				recorrido8 = true;
				movAuto_z = 230.0f;
			}
		}
		if (recorrido8) {
			movAuto_x -= 5.0f;
			movAuto_z -= 5.0f;
			orienta = 310.0f;
			if (movAuto_x <= 600.0f) {
				recorrido8 = false;
				recorrido9 = true;
				movAuto_x = 600.0f;
			}
		}
		if (recorrido9) {
			movAuto_x -= 10.0f;
			orienta = 360.0f;
			if (movAuto_x <= 0.0f) {
				recorrido9 = false;
				recorrido1 = true;
				movAuto_x = 0.0f;
			}
		}


		if (estadoArbol1){
			orientaArbolX += 0.04f;
			orientaArbolY += 0.04f;
			if(orientaArbolX >= 2.0f){
				estadoArbol1 = false;
				estadoArbol2 = true;
			}
		} 
		if (estadoArbol2){
			orientaArbolX -= 0.04f;
			orientaArbolY -= 0.04f;
			if(orientaArbolX <= 0.0f){
				estadoArbol1 = true;
				estadoArbol2 = false;
			}
		}

		/*
		if (estadoHoja1){
			trasladaHojaY = 0.0f;
			estadoHoja1 = false;
		}
		if (estadoHoja2){
			orientaHojaX += 1.0f;
			orientaHojaY += 1.0f;
			trasladaHojaY -= 0.2f;
			if(orientaHojaX >= 9.0f){
				estadoHoja2 = false;
				estadoHoja3 = true;
			}
			if (trasladaHojaY <= -150.0f){
				estadoHoja1 = true;
			}
		} 
		if (estadoHoja3){
			orientaHojaX -= 1.0f;
			orientaHojaY -= 1.0f;
			trasladaHojaY -= 0.2f;
			if(orientaHojaX <= 0.0f){
				estadoHoja2 = true;
				estadoHoja3 = false;
			}
			if (trasladaHojaY <= -150.0f){
				estadoHoja1 = true;
			}
		}
		*/
		if (estadoHoja1) {
			orientaHojaX = 0.0f;
			orientaHojaY = 0.0f;
			trasladaHojaY = 0.0f;
			trasladaHojaZ = 0.0f;
			estadoHoja1 = false;
		}
		if (estadoHoja2) {
			orientaHojaX -= 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ -= 0.8f;
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f){
				estadoHoja2 = false;
				estadoHoja3 = true;
			} 
			if (trasladaHojaY <= -400.0f) {
				estadoHoja1 = true;
			}
		}
		if (estadoHoja3) {
			orientaHojaX += 1.0f;
			orientaHojaY -= 0.5f;
			trasladaHojaZ -= 0.8f;
			if (orientaHojaX >= 5.0f){
				estadoHoja3 = false;
				estadoHoja4 = true;
			} 
		}
		if (estadoHoja4) {
			orientaHojaX += 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ += 0.8f;
			if (trasladaHojaZ >= 40.0f && orientaHojaX >= 5.0f){
				estadoHoja4 = false;
				estadoHoja5 = true;
			}
			if (trasladaHojaY <= -400.0f) {
				estadoHoja1 = true;
			}
		}
		if (estadoHoja5) {
			orientaHojaX -= 1.0f;
			orientaHojaY -= 0.5f;
			trasladaHojaZ += 0.8f;
			if (orientaHojaX <= -5.0f){
				estadoHoja5 = false;
				estadoHoja6 = true;
			} 
		}
		if (estadoHoja6) {
			orientaHojaX -= 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ -= 0.8f;
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f){
				estadoHoja6 = false;
				estadoHoja7 = true;
			}
			if (trasladaHojaY <= -400.0f) {
				estadoHoja1 = true;
			}
		}
		if (estadoHoja7) {
			orientaHojaX += 1.0f;
			orientaHojaY -= 0.5f;
			trasladaHojaZ -= 0.8f;
			if (orientaHojaX >= 5.0f){
				estadoHoja7 = false;
				estadoHoja8 = true;
			} 
		}
		if (estadoHoja8) {
			orientaHojaX += 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ += 0.8f;
			if (trasladaHojaZ >= 40.0f && orientaHojaX >= 5.0f){
				estadoHoja8 = false;
				estadoHoja9 = true;
			}
			if (trasladaHojaY <= -400.0f) {
				estadoHoja1 = true;
			}
		}
		if (estadoHoja9) {
			orientaHojaX -= 1.0f;
			orientaHojaY -= 0.5f;
			trasladaHojaZ += 0.8f;
			if (orientaHojaX <= -10.0f){
				estadoHoja9 = false;
				estadoHoja10 = true;
			} 
		}
		if (estadoHoja10) {
			orientaHojaX -= 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ -= 0.8f;
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f){
				estadoHoja10 = false;
				estadoHoja2 = true;
			}
			if (trasladaHojaY <= -400.0f) {
				estadoHoja1 = true;
			}
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// ----------------------------------------------------------
	// load models
	// ----------------------------------------------------------

	Model piso("resources/objects/Christian/Parque/pisoFraccionamiento.obj");
	Model EstacionBus("resources/objects/Christian/EstacionBus/EstacionBus.obj");
	Model Bus("resources/objects/Christian/Bus/Bus.obj");
	Model Arbol("resources/objects/Christian/Arbol/Arbol.obj");
	Model ArbolVerde("resources/objects/Christian/arbolVerde/arbolVerde.obj");
	//Model Perro("resources/objects/Pablo/perro/perroProto.obj");
	Model hojasArbol("resources/objects/Christian/Arbol/hojasArbol.obj");
	Model tronco("resources/objects/Christian/Arbol/tronco.obj");
	Model hojasCaidas("resources/objects/Christian/hojasCaidas/hojasCaidas.obj");

	// Audio Configuration

	if (!Background)
		return 0;

	Background->setSoundVolume(0.2);
	Background->play2D("sound/Background.mp3", true);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Character animation
		// -------------------------------------------------------------------------------------------------------------------------


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		/*********************************** PISO **********************************/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.25f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		/****************************** ÁRBOLES PARQUE *****************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 30.0f, 315.0f));				//árbol animado (zona 1)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 0.0f, 315.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(260.0f, 30.0f, 860.0f));				//árbol animado (zona 1)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(260.0f, 0.0f, 860.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(330.0f, 30.0f, 540.0f));				//árbol animado (zona 1)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(330.0f, 0.0f, 540.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, 30.0f, 610.5f));				//árbol animado (zona 1)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, 0.0f, 610.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 30.0f, 315.0f));				//árbol animado (zona 2)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 0.0f, 315.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260.0f, 30.0f, 860.0f));				//árbol animado (zona 2)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260.0f, 0.0f, 860.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 30.0f, 540.5f));				//árbol animado (zona 2)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 540.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, 30.0f, 610.5f));				//árbol animado (zona 2)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, 0.0f, 610.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 30.0f, -330.0f));				//árbol animado (zona 3)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(650.0f, 0.0f, -330.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(260.0f, 30.0f, -860.0f));				//árbol animado (zona 3)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(260.0f, 0.0f, -860.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 30.0f, -550.5f));				//árbol animado (zona 3)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, -550.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, 30.0f, -610.5f));				//árbol animado (zona 3)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, 0.0f, -610.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 30.0f, -330.0f));				//árbol animado (zona 4)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, 0.0f, -330.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260.0f, 30.0f, -860.0f));				//árbol animado (zona 4)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-260.0f, 0.0f, -860.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, -550.5f));					//árbol estático (zona 4)
		model = glm::scale(model, glm::vec3(1.0f, 1.3f, 1.0f));
		staticShader.setMat4("model", model);
		Arbol.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 30.0f, -550.5f));				//árbol animado (zona 4)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, -550.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, 30.0f, -610.5f));				//árbol animado (zona 4)
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaArbolX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaArbolY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasArbol.Draw(staticShader);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, 0.0f, -610.5f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		staticShader.setMat4("model", model);
		tronco.Draw(staticShader);

		/********************************* HOJAS CAIDAS PARQUE ********************************/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(590.0f, 400.0f + trasladaHojaY, 440.0f + trasladaHojaZ));					//hojas caídas (zona 1)
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(310.0f, 250.0f + trasladaHojaY, 700.0f + trasladaHojaZ));					//hojas caídas (zona 1)
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(410.0f, 320.0f + trasladaHojaY, 550.0f + trasladaHojaZ));					//hojas caídas (zona 1)
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(480.0f, 360.0f + trasladaHojaY, 630.0f + trasladaHojaZ));					//hojas caídas (zona 1)
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-590.0f, 400.0f + trasladaHojaY, 440.0f + trasladaHojaZ));					//hojas caídas (zona 2)
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-310.0f, 300.0f + trasladaHojaY, 700.0f + trasladaHojaZ));					//hojas caídas (zona 2)
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-410.0f, 340.0f + trasladaHojaY, 550.0f + trasladaHojaZ));					//hojas caídas (zona 2)
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-480.0f, 280.0f + trasladaHojaY, 630.0f + trasladaHojaZ));					//hojas caídas (zona 2)
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(590.0f, 400.0f + trasladaHojaY, -440.0f + trasladaHojaZ));					//hojas caídas (zona 3)
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(310.0f, 340.0f + trasladaHojaY, -700.0f + trasladaHojaZ));					//hojas caídas (zona 3)
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(410.0f, 260.0f + trasladaHojaY, -550.0f + trasladaHojaZ));					//hojas caídas (zona 3)
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(480.0f, 300.0f + trasladaHojaY, -630.0f + trasladaHojaZ));					//hojas caídas (zona 3)
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-590.0f, 400.0f + trasladaHojaY, -440.0f + trasladaHojaZ));					//hojas caídas (zona 4)
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-310.0f, 330.0f + trasladaHojaY, -700.0f + trasladaHojaZ));					//hojas caídas (zona 4)
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-410.0f, 250.0f + trasladaHojaY, -550.0f + trasladaHojaZ));					//hojas caídas (zona 4)
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-480.0f, 290.0f + trasladaHojaY, -630.0f + trasladaHojaZ));					//hojas caídas (zona 4)
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, glm::radians(orientaHojaX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaHojaY), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		hojasCaidas.Draw(staticShader);


		/****************************** ÁRBOLES ALREDEDOR *****************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1300.0f, 0.0f, 1000.0f)); 		//árbol alrededor (zona 1)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1600.0f, 0.0f, 1600.0f));			//árbol alrededor (zona 1)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1300.0f, 0.0f, 1000.0f)); 		//árbol alrededor (zona 2)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1600.0f, 0.0f, 1600.0f));			//árbol alrededor (zona 2)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1300.0f, 0.0f, -1000.0f)); 		//árbol alrededor (zona 3)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1600.0f, 0.0f, -1600.0f));			//árbol alrededor (zona 3)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);


		/********************************* ESTACIÓN BUS ********************************/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1350.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		EstacionBus.Draw(staticShader);

		/************************************** BUS *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f+movAuto_x, -30.0f, -1060.0f+ movAuto_z));
		model = glm::scale(model, glm::vec3(15.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Bus.Draw(staticShader);

		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(120.0f, 30.0f, 180.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Perro.Draw(staticShader);*/


		// -------------------------------------------------------------------------------------------------------------------------
		// Map elements
		// -------------------------------------------------------------------------------------------------------------------------






		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Background->drop(); // delete Background Sound Engine
	return 0;

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}