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
float	movAuto_x = 0.0f,	// para la animación del autobús
		movAuto_z = 0.0f,
		orienta = 0.0f,
		orientaArbolX = 0.0f,	// para la animación del árbol
		orientaArbolY = 0.0f,
		orientaArbolZ = 0.0f,
		orientaHojaX = 0.0f,	// para la animación de las hojas caídas
		orientaHojaY = 0.0f,
		trasladaHojaY = 0.0f,
		trasladaHojaZ = 0.0f,
		orientaPersonajeH_Y = 0.0f,	// para la animación del personaje M
		trasladaPersonajeH_X = 0.0f,
		trasladaPersonajeH_Z = 0.0f,
		trasladaAguilaX = 0.0f,		// para la animación del águila
		trasladaAguilaZ = 0.0f,
		orientaAlasAguilaY = 0.0f,
		orientaAlasAguilaX = 0.0f,
		patoOrientacion = 0.0f,
		patoIncremento = 0.0f;

//Pato posicion
glm::vec3 patoPos(0.0f, 0.0f, 0.0f);
		
bool	animacion1 = false,
		animacion2 = false,
		animacion3 = false,
		animacion4 = false,
		animacion5 = false,
		recorrido1 = true,	// para la animación del autobús
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false,
		recorrido7 = false,
		recorrido8 = false,
		recorrido9 = false,
		estadoArbol1 = true, // para la animación del árbol
		estadoArbol2 = false,
		estadoArbol3 = false,
		estadoArbol4 = false,
		estadoArbol5 = false,
		estadoArbol6 = false,
		estadoHoja1 = false, // para la animación de las hojas caídas
		estadoHoja2 = true,
		estadoHoja3 = false,
		estadoHoja4 = false,
		estadoHoja5 = false,
		estadoHoja6 = false,
		estadoHoja7 = false,
		estadoHoja8 = false,
		estadoHoja9 = false,
		estadoHoja10 = false,
		estadoPersonajeM1 = true, // para la animación del personaje M
		estadoPersonajeM2 = false,
		estadoPersonajeM3 = false,
		estadoPersonajeM4 = false,
		estadoPersonajeM5 = false,
		estadoPersonajeM6 = false,
		estadoAguila1 = true,		// para la animación del águila
		estadoAguila2 = false,
		estadoAguila3 = true,
		estadoAguila4 = false,
		estadoAguila5 = false,
		estadoAguila6 = false;

int		walkManState= 0;

float	walkManAngle = 270.0f,
		movWalkManX = 1150.0f,
		movWalkManZ = 500.0f,
		incremFactor = 6.0f;

bool turnOnWalkMan = false;

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

	//********************************* ANIMACIÓN DEL AUTOBÚS *************************************
	if (animacion1)
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
	}
	
	//******************************************ANIMACIÓN DEL ÁRBOL *******************************
	if(animacion2){
		if (estadoArbol1){
			orientaArbolX += 0.08f;
			orientaArbolY += 0.08f;
			if(orientaArbolX >= 3.0f){
				estadoArbol1 = false;
				estadoArbol2 = true;
			}
		} 
		if (estadoArbol2){
			orientaArbolX -= 0.08f;
			orientaArbolY -= 0.08f;
			if(orientaArbolX <= 0.0f){
				estadoArbol2 = false;
				estadoArbol3 = true;
			}
		}
		if (estadoArbol3){
			orientaArbolZ += 0.08f;
			orientaArbolY += 0.08f;
			if(orientaArbolZ >= 3.0f){
				estadoArbol3 = false;
				estadoArbol4 = true;
			}
		}
		if (estadoArbol4){
			orientaArbolZ -= 0.08f;
			orientaArbolY -= 0.08f;
			if(orientaArbolZ <= 0.0f){
				estadoArbol4 = false;
				estadoArbol5 = true;
			}
		}
		if (estadoArbol5){
			orientaArbolX += 0.12f;
			orientaArbolY += 0.12f;
			if(orientaArbolX >= 4.0f){
				estadoArbol5 = false;
				estadoArbol6 = true;
			}
		} 
		if (estadoArbol6){
			orientaArbolX -= 0.07f;
			orientaArbolY -= 0.07f;
			if(orientaArbolX <= 0.0f){
				estadoArbol6 = false;
				estadoArbol1 = true;
			}
		}
	}

	// ******************************** ANIMACIÓN DE LAS HOJAS CAÍDAS ******************************
	if (animacion3){
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

	// ********************************* ANIMACIÓN DEL PERDONAJE M *******************************
		if (estadoPersonajeM1) {
			trasladaPersonajeH_Z += 6.0f;
			trasladaPersonajeH_X = 0.0f;
			orientaPersonajeH_Y = 0.0f;
			if (trasladaPersonajeH_Z >= 650.0f){
				estadoPersonajeM1 = false;
				estadoPersonajeM2 = true;
				trasladaPersonajeH_Z = 650.0f;
			}
		}
		if (estadoPersonajeM2) {
			trasladaPersonajeH_Z += 0.0f;
			trasladaPersonajeH_X += 6.0f;
			orientaPersonajeH_Y = 90.0f;
			if (trasladaPersonajeH_X >= 650.0f){
				estadoPersonajeM2 = false;
				estadoPersonajeM3 = true;
				trasladaPersonajeH_X = 650.0f;
			}
		}
		if (estadoPersonajeM3) {
			trasladaPersonajeH_Z -= 4.0f;
			trasladaPersonajeH_X += 4.0f;
			orientaPersonajeH_Y = 135.0f;
			if (trasladaPersonajeH_X >= 950.0f) {
				estadoPersonajeM3 = false;
				estadoPersonajeM4 = true;
				trasladaPersonajeH_X = 950.0f;
			}
		}
		if (estadoPersonajeM4) {
			trasladaPersonajeH_Z -= 6.0f;
			trasladaPersonajeH_X += 0.0f;
			orientaPersonajeH_Y = 180.0f;
			if (trasladaPersonajeH_Z <= -600.0f) {
				estadoPersonajeM4 = false;
				estadoPersonajeM5 = true;
				trasladaPersonajeH_Z = -600.0f;
			}
		}
		if (estadoPersonajeM5) {
			trasladaPersonajeH_Z += 0.0f;
			trasladaPersonajeH_X -= 6.0f;
			orientaPersonajeH_Y = 270.0f;
			if (trasladaPersonajeH_X <= 400.0f) {
				estadoPersonajeM5 = false;
				estadoPersonajeM6 = true;
				trasladaPersonajeH_X = 400.0f;
			}
		}
		if (estadoPersonajeM6) {
			trasladaPersonajeH_Z += 4.0f;
			trasladaPersonajeH_X -= 4.0f;
			orientaPersonajeH_Y = 315.0f;
			if (trasladaPersonajeH_X <= 0.0f) {
				estadoPersonajeM6 = false;
				estadoPersonajeM1 = true;
				trasladaPersonajeH_X = 0.0f;
			}
		}
	
	//********************************** ANIMACIÓN DEL ÁGUILA **********************************
	if(animacion5){
		if(estadoAguila1){
			orientaAlasAguilaY += 5.0f;
			if(orientaAlasAguilaY >= 30.0f){
				estadoAguila1 = false;
				estadoAguila2 = true;
			}
		}
		if(estadoAguila2){
			orientaAlasAguilaY -= 5.0f;
			if(orientaAlasAguilaY <= 0.0f){
				estadoAguila2 = false;
				estadoAguila1 = true;
			}
		}
		if(estadoAguila3){
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 0.0f;
			if(trasladaAguilaX >= 1500.0f){
				estadoAguila3 = false;
				estadoAguila4 = true;
			}
		}
		if(estadoAguila4){
			trasladaAguilaX += 0.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 45.0f;
			if(trasladaAguilaZ >= 3000.0f){
				estadoAguila4 = false;
				estadoAguila5 = true;
			}
		}
		if(estadoAguila5){
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 0.0f;
			if(trasladaAguilaZ >= 4500.0f){
				estadoAguila5 = false;
				estadoAguila6 = true;
			}
		}
		if(estadoAguila6){
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 0.0f;
			orientaAlasAguilaX = -45.0f;
			if(trasladaAguilaX >= 4000.0f){
				estadoAguila6 = false;
				estadoAguila3 = true;
				trasladaAguilaX = 0.0f;
				trasladaAguilaZ = 0.0f;
				orientaAlasAguilaX = 0.0f;
			}
		}
	}

	//********************************** Walking Man **********************************
	if(turnOnWalkMan){

		if (walkManState == 0) {
			movWalkManX -= incremFactor;
			walkManAngle = 270.0f;
			if (movWalkManX <= 1100) walkManState = 1;
		}

		if (walkManState == 1) {
			movWalkManZ -= incremFactor;
			walkManAngle = 180.0f;
			if (movWalkManZ <= -1500) walkManState = 2;
		}

		if (walkManState == 2) {
			movWalkManX -= incremFactor;
			walkManAngle = 270.0f;
			if (movWalkManX <= -1100) walkManState = 3;
		}

		if (walkManState == 3) {
			movWalkManZ += incremFactor;
			walkManAngle = 0.0f;
			if (movWalkManZ >= -400) walkManState = 4;
		}

		if (walkManState == 4) {
			movWalkManX += incremFactor;
			walkManAngle = 90.0f;
			if (movWalkManX >= -950) walkManState = 5;
		}

		if (walkManState == 5) {
			movWalkManZ += incremFactor;
			walkManAngle = 0.0f;
			if (movWalkManZ >= -150) walkManState = 6;
		}

		if (walkManState == 6) {
			movWalkManX -= incremFactor;
			walkManAngle = 270.0f;
			if (movWalkManX <= -1050) walkManState = 7;
		}

		if (walkManState == 7) {
			movWalkManZ += incremFactor;
			walkManAngle = 0.0f;
			if (movWalkManZ >= 0) walkManState = 8;
		}

		if (walkManState == 8) {
			movWalkManZ -= incremFactor;
			walkManAngle = 180.0f;
			if (movWalkManZ <= -150) walkManState = 9;
		}

		if (walkManState == 9) {
			movWalkManX += incremFactor;
			walkManAngle = 90.0f;
			if (movWalkManX >= -300) walkManState = 10;
		}

		if (walkManState == 10) {
			movWalkManZ -= incremFactor;
			walkManAngle = 180.0f;
			if (movWalkManZ <= -320) walkManState = 11;
		}

		if (walkManState == 11) {
			movWalkManX += incremFactor;
			walkManAngle = 90.0f;
			if (movWalkManX >= 345) walkManState = 12;
		}
		if (walkManState == 12) {
			movWalkManZ += 0.98170732f * 6.0f;
			movWalkManX += 0.98170732f * 6.0f;
			walkManAngle = 45.53f;
			if (movWalkManX >= 1150 && movWalkManZ >= 500) walkManState = 0; 
		}
	}

	//Animación Pato
	patoIncremento -= 0.0085f;
	patoPos.x = 130.0f * cos(patoIncremento);
	patoPos.z = 130.0f * sin(patoIncremento);
	patoOrientacion += 0.49f;

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

	Model piso("resources/objects/Christian/pisoFraccionamiento/pisoFraccionamiento.obj");
	Model EstacionBus("resources/objects/Christian/EstacionBus/EstacionBus.obj");
	Model Bus("resources/objects/Christian/Bus/Bus.obj");
	Model Arbol("resources/objects/Christian/Arbol/Arbol.obj");
	Model ArbolVerde("resources/objects/Christian/arbolVerde/arbolVerde.obj");
	Model perro("resources/objects/Pablo/perro/perro.obj");
	Model hojasArbol("resources/objects/Christian/Arbol/hojasArbol.obj");
	Model tronco("resources/objects/Christian/Arbol/tronco.obj");
	Model hojasCaidas("resources/objects/Christian/hojasCaidas/hojasCaidas.obj");
	Model fuente("resources/objects/Christian/fuente/fuente.obj");
	Model lampara("resources/objects/Christian/lampara/lampara.obj");
	Model cuerpo("resources/objects/Christian/eagle/cuerpo.obj");
	Model alaIzquierda("resources/objects/Christian/eagle/alaIzquierda.obj");
	Model alaDerecha("resources/objects/Christian/eagle/alaDerecha.obj");
	Model Edificio("resources/objects/Diego/edificio/edificio.obj");
	Model Kiosko("resources/objects/Diego/kiosko1/kiosko.obj");
	Model pato("resources/objects/Pablo/pato/pato.obj");
	ModelAnim personajeM("resources/objects/Christian/personajeM/personajeM.dae");
	personajeM.initShaders(animShader.ID);
	ModelAnim personajeH("resources/objects/Christian/personajeH/personajeH.dae");
	personajeH.initShaders(animShader.ID);
	//Gallery
	Model gallery("resources/objects/Elizabeth/galeria/galeria.obj");
	//House
	Model house("resources/objects/Elizabeth/casa4x4/casa4x4.obj");

	//Walking man
	ModelAnim walkingMan("resources/objects/Juan/walking_man/Walking.dae");
	walkingMan.initShaders(animShader.ID);


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

		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		/********************************* PERSONAJE M ********************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + trasladaPersonajeH_X, 4.0f, 600.0f + trasladaPersonajeH_Z));
		model = glm::rotate(model, glm::radians(orientaPersonajeH_Y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		animShader.setMat4("model", model);
		personajeM.Draw(animShader);

		/********************************* PERSONAJE H ********************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, -1610.0f));
		model = glm::scale(model, glm::vec3(0.55f));
		animShader.setMat4("model", model);
		personajeH.Draw(animShader);

		/********************************* Walking Man ********************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(movWalkManX, 0.0f, movWalkManZ));
		model = glm::rotate(model, glm::radians(walkManAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		animShader.setMat4("model", model);
		walkingMan.Draw(animShader);


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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::rotate(model, glm::radians(orientaArbolZ), glm::vec3(0.0f, 1.0f, 0.0f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1600.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		EstacionBus.Draw(staticShader);

		/************************************** BUS *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f+movAuto_x, -30.0f, -1060.0f+ movAuto_z));
		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Bus.Draw(staticShader);

		/************************************** FUENTE *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		fuente.Draw(staticShader);

		/*************************************** PATO **************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(patoPos.x, 30.0f + patoPos.y, patoPos.z));
		model = glm::rotate(model, glm::radians(270.0f + patoOrientacion), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		pato.Draw(staticShader);

		/*************************************** PERRO *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 10.0f, 400.0f));
		model = glm::rotate(model, glm::radians(139.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		perro.Draw(staticShader);
		
		/************************************  Edificio *************************************/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(850.0f, 0.0f, 2500.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Edificio.Draw(staticShader);
		glEnable(GL_BLEND);
		
		/************************************  Kiosko *************************************/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, 1900.0f));
		model = glm::scale(model, glm::vec3(9.0f));
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Kiosko.Draw(staticShader);
		
		/************************************** LÁMPARAS *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 0.0f, -500.0f));			//lámpara alrededor (zona 1)
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 0.0f, 500.0f));				//lámpara alrededor (zona 1)
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, -500.0f));				//lámpara alrededor (zona 2)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, 800.0f));				//lámpara alrededor (zona 2)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 0.0f, -2100.0f));		//lámpara alrededor (zona 2)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, 1300.0f));			//lámpara alrededor (zona 3)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);
			
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-600.0f, 0.0f, 1300.0f));			//lámpara alrededor (zona 3)
		model = glm::scale(model, glm::vec3(3.5f));	
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1250.0f, 0.0f, 2150.0f));			//lámpara alrededor (zona 3)
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1250.0f, 0.0f, 2150.0f));		//lámpara alrededor (zona 3)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, -1300.0f));			//lámpara alrededor (zona 4)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-600.0f, 0.0f, -1300.0f));		//lámpara alrededor (zona 4)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1250.0f, 0.0f, -2150.0f));		//lámpara alrededor (zona 5)
		model = glm::scale(model, glm::vec3(3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lampara.Draw(staticShader);

		/************************************** Gallery *************************************/
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -590.0f));
		model = glm::scale(model, glm::vec3(1.7f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		gallery.Draw(staticShader);

		
		
		/************************************** Houses *************************************/

		//House 1
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 520.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 2
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 3
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 4
		model = glm::translate(model, glm::vec3(65.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		/************************************** Águila *************************************/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2000.0f + trasladaAguilaX, 1000.0f, -2500.0f + trasladaAguilaZ));
		model = glm::scale(model, glm::vec3(1.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaAlasAguilaX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		cuerpo.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2000.0f + trasladaAguilaX, 1000.0f, -2500.0f + trasladaAguilaZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaAlasAguilaX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaAlasAguilaY), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		alaIzquierda.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-2000.0f + trasladaAguilaX, 1000.0f, -2500.0f + trasladaAguilaZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaAlasAguilaX), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(orientaAlasAguilaY), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		alaDerecha.Draw(staticShader);

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

	//Bus animation
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
		animacion1 ^= true;
	//Tree animation 
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
		animacion2 ^= true;
	//Tree leaves
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		animacion3 ^= true;
	//eagle animation
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		animacion5 ^= true;

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
