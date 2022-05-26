// PROYECTO FINAL LAB. CGeIHC
//INTEGRANTES:
//González Blando Pablo
//Mendoza de la Vega Dulce Elizabeth
//Rodea Coria Christian
//Sánchez Rojo Juan Pablo
//Tejada Orozco Diego Francisco

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
GLFWmonitor* monitors;

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
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//Count 
float countAux = 0.0f,
variableX = 0.0f,
variableY = 0.0f,
variableZ = 0.0f;
boolean anochecer = true;

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

//Walking Man
int		walkManState = 0;
float	walkManAngle = 270.0f,
movWalkManX = 1150.0f,
movWalkManZ = 500.0f,
incremFactor = 6.0f;
bool	turnOnWalkMan = true;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f;

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
	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	rotRodIzq = KeyFrame[0].rotRodIzq;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
}

void animate(void)
{
	if (anochecer)
	{
		variableX += 0.01f;
		variableY += 0.01f;
		variableZ += 0.01f;
		if (variableX >= 1.0f || variableY >= 1.0f || variableZ >= 1.0f) {
			anochecer = false;
		}
	}

	if (!anochecer) {

		variableX -= 0.001f;
		variableY -= 0.001f;
		variableZ -= 0.001f;
		if (variableX <= 0.05f || variableY <= 0.05f || variableZ <= 0.05f) {
			anochecer = true;
		}
	}
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
	if (animacion2) {
		if (estadoArbol1) {
			orientaArbolX += 0.08f;
			orientaArbolY += 0.08f;
			if (orientaArbolX >= 3.0f) {
				estadoArbol1 = false;
				estadoArbol2 = true;
			}
		}
		if (estadoArbol2) {
			orientaArbolX -= 0.08f;
			orientaArbolY -= 0.08f;
			if (orientaArbolX <= 0.0f) {
				estadoArbol2 = false;
				estadoArbol3 = true;
			}
		}
		if (estadoArbol3) {
			orientaArbolZ += 0.08f;
			orientaArbolY += 0.08f;
			if (orientaArbolZ >= 3.0f) {
				estadoArbol3 = false;
				estadoArbol4 = true;
			}
		}
		if (estadoArbol4) {
			orientaArbolZ -= 0.08f;
			orientaArbolY -= 0.08f;
			if (orientaArbolZ <= 0.0f) {
				estadoArbol4 = false;
				estadoArbol5 = true;
			}
		}
		if (estadoArbol5) {
			orientaArbolX += 0.12f;
			orientaArbolY += 0.12f;
			if (orientaArbolX >= 4.0f) {
				estadoArbol5 = false;
				estadoArbol6 = true;
			}
		}
		if (estadoArbol6) {
			orientaArbolX -= 0.07f;
			orientaArbolY -= 0.07f;
			if (orientaArbolX <= 0.0f) {
				estadoArbol6 = false;
				estadoArbol1 = true;
			}
		}
	}

	// ******************************** ANIMACIÓN DE LAS HOJAS CAÍDAS ******************************
	if (animacion3) {
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
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f) {
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
			if (orientaHojaX >= 5.0f) {
				estadoHoja3 = false;
				estadoHoja4 = true;
			}
		}
		if (estadoHoja4) {
			orientaHojaX += 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ += 0.8f;
			if (trasladaHojaZ >= 40.0f && orientaHojaX >= 5.0f) {
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
			if (orientaHojaX <= -5.0f) {
				estadoHoja5 = false;
				estadoHoja6 = true;
			}
		}
		if (estadoHoja6) {
			orientaHojaX -= 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ -= 0.8f;
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f) {
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
			if (orientaHojaX >= 5.0f) {
				estadoHoja7 = false;
				estadoHoja8 = true;
			}
		}
		if (estadoHoja8) {
			orientaHojaX += 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ += 0.8f;
			if (trasladaHojaZ >= 40.0f && orientaHojaX >= 5.0f) {
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
			if (orientaHojaX <= -10.0f) {
				estadoHoja9 = false;
				estadoHoja10 = true;
			}
		}
		if (estadoHoja10) {
			orientaHojaX -= 0.5f;
			orientaHojaY -= 0.5f;
			trasladaHojaY -= 0.8f;
			trasladaHojaZ -= 0.8f;
			if (trasladaHojaZ <= -40.0f && orientaHojaX <= -5.0f) {
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
		if (trasladaPersonajeH_Z >= 650.0f) {
			estadoPersonajeM1 = false;
			estadoPersonajeM2 = true;
			trasladaPersonajeH_Z = 650.0f;
		}
	}
	if (estadoPersonajeM2) {
		trasladaPersonajeH_Z += 0.0f;
		trasladaPersonajeH_X += 6.0f;
		orientaPersonajeH_Y = 90.0f;
		if (trasladaPersonajeH_X >= 650.0f) {
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
	if (animacion5) {
		if (estadoAguila1) {
			orientaAlasAguilaY += 5.0f;
			if (orientaAlasAguilaY >= 30.0f) {
				estadoAguila1 = false;
				estadoAguila2 = true;
			}
		}
		if (estadoAguila2) {
			orientaAlasAguilaY -= 5.0f;
			if (orientaAlasAguilaY <= 0.0f) {
				estadoAguila2 = false;
				estadoAguila1 = true;
			}
		}
		if (estadoAguila3) {
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 0.0f;
			if (trasladaAguilaX >= 1500.0f) {
				estadoAguila3 = false;
				estadoAguila4 = true;
			}
		}
		if (estadoAguila4) {
			trasladaAguilaX += 0.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 45.0f;
			if (trasladaAguilaZ >= 3000.0f) {
				estadoAguila4 = false;
				estadoAguila5 = true;
			}
		}
		if (estadoAguila5) {
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 10.0f;
			orientaAlasAguilaX = 0.0f;
			if (trasladaAguilaZ >= 4500.0f) {
				estadoAguila5 = false;
				estadoAguila6 = true;
			}
		}
		if (estadoAguila6) {
			trasladaAguilaX += 10.0f;
			trasladaAguilaZ += 0.0f;
			orientaAlasAguilaX = -45.0f;
			if (trasladaAguilaX >= 4000.0f) {
				estadoAguila6 = false;
				estadoAguila3 = true;
				trasladaAguilaX = 0.0f;
				trasladaAguilaZ = 0.0f;
				orientaAlasAguilaX = 0.0f;
			}
		}
	}

	//********************************** Walking Man Animation **********************************
	if (turnOnWalkMan) {

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
			movWalkManZ += incremFactor;
			walkManAngle = 0.0f;
			if (movWalkManZ >= -250) walkManState = 5;
		}

		if (walkManState == 5) {
			movWalkManX -= incremFactor;
			walkManAngle = 270.0f;
			if (movWalkManX <= -1250) walkManState = 6;
		}

		if (walkManState == 6) {
			movWalkManZ += incremFactor;
			walkManAngle = 0.0f;
			if (movWalkManZ >= -330) walkManState = 7;
		}

		if (walkManState == 7) {
			movWalkManZ -= incremFactor;
			walkManAngle = 180.0f;
			if (movWalkManZ <= -150) walkManState = 8;
		}

		if (walkManState == 8) {
			movWalkManX += incremFactor;
			walkManAngle = 90.0f;
			if (movWalkManX >= -300) walkManState = 9;
		}

		if (walkManState == 9) {
			movWalkManZ -= incremFactor;
			walkManAngle = 180.0f;
			if (movWalkManZ <= -320) walkManState = 10;
		}

		if (walkManState == 10) {
			movWalkManX += incremFactor;
			walkManAngle = 90.0f;
			if (movWalkManX >= 345) walkManState = 11;
		}
		if (walkManState == 11) {
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
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

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
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
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
	Model mesaExterior("resources/objects/Christian/mesaExterior/mesaExterior.obj");
	Model banca("resources/objects/Christian/banca/banca.obj");
	Model Edificio("resources/objects/Diego/edificio/edificio.obj");
	Model Kiosko("resources/objects/Diego/kiosko1/kiosko.obj");
	Model pato("resources/objects/Pablo/pato/pato.obj");
	ModelAnim personajeM("resources/objects/Christian/personajeM/personajeM.dae");
	personajeM.initShaders(animShader.ID);
	ModelAnim personajeH("resources/objects/Christian/personajeH/personajeH.dae");
	personajeH.initShaders(animShader.ID);
	//Gallery
	Model gallery("resources/objects/Elizabeth/galeria/galeria.obj");
	Model doorGallery("resources/objects/Elizabeth/puerta_galeria/puerta_galeria.obj");
	//House
	Model house("resources/objects/Elizabeth/casa4x4/casa4x4.obj");
	Model doorHouse("resources/objects/Elizabeth/puerta_casa/puerta_casa.obj");
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
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(variableX, variableY, variableZ));
		//En caso de ver las luces externas
		//staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		
		staticShader.setVec3("pointLight[0].position", glm::vec3(-859.0f, 290.0f, -500.0f));  //luz lámpara alrededor (zona 1)
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[0].constant", 1.0f);
		staticShader.setFloat("pointLight[0].linear", 0.00009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-859.0f, 290.0f, 500.0f));  //luz lámpara alrededor (zona 1)
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.000009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0002f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(859.0f, 290.0f, -500.0f));    //luz lámpara alrededor (zona 2)
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[3].position", glm::vec3(859.0f, 290.0f, 800.0f));     //luz lámpara alrededor (zona 2)
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[3].constant", 1.0f);
		staticShader.setFloat("pointLight[3].linear", 0.009f);
		staticShader.setFloat("pointLight[3].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[4].position", glm::vec3(859.0f, 290.0f, -2100.0f));     //luz lámpara alrededor (zona 2)
		staticShader.setVec3("pointLight[4].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[4].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[4].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[4].constant", 1.0f);
		staticShader.setFloat("pointLight[4].linear", 0.009f);
		staticShader.setFloat("pointLight[4].quadratic", 0.00032f);


		staticShader.setVec3("pointLight[5].position", glm::vec3(600.0f, 290.0f, 1159.0f));     //luz lámpara alrededor (zona 3)
		staticShader.setVec3("pointLight[5].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[5].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[5].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[5].constant", 1.0f);
		staticShader.setFloat("pointLight[5].linear", 0.009f);
		staticShader.setFloat("pointLight[5].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[6].position", glm::vec3(-600.0f, 290.0f, 1159.0f));     //luz lámpara alrededor (zona 3)
		staticShader.setVec3("pointLight[6].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[6].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[6].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[6].constant", 1.0f);
		staticShader.setFloat("pointLight[6].linear", 0.009f);
		staticShader.setFloat("pointLight[6].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[7].position", glm::vec3(1391.0f, 290.0f, 2150.0f));     //luz lámpara alrededor (zona 3)
		staticShader.setVec3("pointLight[7].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[7].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[7].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[7].constant", 1.0f);
		staticShader.setFloat("pointLight[7].linear", 0.009f);
		staticShader.setFloat("pointLight[7].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[8].position", glm::vec3(-1391.0f, 290.0f, 2150.0f));     //luz lámpara alrededor (zona 3)
		staticShader.setVec3("pointLight[8].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[8].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[8].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[8].constant", 1.0f);
		staticShader.setFloat("pointLight[8].linear", 0.009f);
		staticShader.setFloat("pointLight[8].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[9].position", glm::vec3(600.0f, 290.0f, -1159.0f));     //luz lámpara alrededor (zona 4)
		staticShader.setVec3("pointLight[9].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[9].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[9].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[9].constant", 1.0f);
		staticShader.setFloat("pointLight[9].linear", 0.009f);
		staticShader.setFloat("pointLight[9].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[10].position", glm::vec3(-600.0f, 290.0f, -1159.0f));     //luz lámpara alrededor (zona 4)
		staticShader.setVec3("pointLight[10].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[10].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[10].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[10].constant", 1.0f);
		staticShader.setFloat("pointLight[10].linear", 0.009f);
		staticShader.setFloat("pointLight[10].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[11].position", glm::vec3(-1391.0f, 290.0f, -2150.0f));     //luz lámpara alrededor (zona 5)
		staticShader.setVec3("pointLight[11].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[11].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[11].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[11].constant", 1.0f);
		staticShader.setFloat("pointLight[11].linear", 0.009f);
		staticShader.setFloat("pointLight[11].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[12].position", glm::vec3(-1440.0f, 280.0f, -330.0f));     //luz lámpara interna galeria
		staticShader.setVec3("pointLight[12].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[12].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[12].constant", 1.0f);
		staticShader.setFloat("pointLight[12].linear", 0.009f);
		staticShader.setFloat("pointLight[12].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[13].position", glm::vec3(-1440.0f, 280.0f, 210.0f));     //luz lámpara interna galeria
		staticShader.setVec3("pointLight[13].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[13].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[13].constant", 1.0f);
		staticShader.setFloat("pointLight[13].linear", 0.009f);
		staticShader.setFloat("pointLight[13].quadratic", 0.00032f);


		staticShader.setVec3("pointLight[14].position", glm::vec3(1280.0f, 210.0f, -500.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("pointLight[14].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[14].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[14].ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		staticShader.setVec3("pointLight[14].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[14].constant", 1.0f);
		staticShader.setFloat("pointLight[14].linear", 0.009f);
		staticShader.setFloat("pointLight[14].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[15].position", glm::vec3(1660.0f, 210.0f, -40.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("pointLight[15].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[15].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[15].constant", 1.0f);
		staticShader.setFloat("pointLight[15].linear", 0.009f);
		staticShader.setFloat("pointLight[15].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[16].position", glm::vec3(1260.0f, 210.0f, -40.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("pointLight[16].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[16].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[16].constant", 1.0f);
		staticShader.setFloat("pointLight[16].linear", 0.009f);
		staticShader.setFloat("pointLight[16].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[17].position", glm::vec3(1285.0f, 210.0f, 270.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("pointLight[17].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[17].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[17].constant", 1.0f);
		staticShader.setFloat("pointLight[17].linear", 0.009f);
		staticShader.setFloat("pointLight[17].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[18].position", glm::vec3(1689.0f, 210.0f, 675.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("pointLight[18].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[18].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[18].constant", 1.0f);
		staticShader.setFloat("pointLight[18].linear", 0.009f);
		staticShader.setFloat("pointLight[18].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[19].position", glm::vec3(1285.0f, 210.0f, 675.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("pointLight[19].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[19].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[19].constant", 1.0f);
		staticShader.setFloat("pointLight[19].linear", 0.009f);
		staticShader.setFloat("pointLight[19].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[20].position", glm::vec3(1700.0f, 520.0f, -330.0f));     //luz lámpara interna casa 2 piso 2
		staticShader.setVec3("pointLight[20].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[20].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[20].constant", 1.0f);
		staticShader.setFloat("pointLight[20].linear", 0.009f);
		staticShader.setFloat("pointLight[20].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[21].position", glm::vec3(1730.0f, 520.0f, 420.0f));     //luz lámpara interna casa 1 piso 2
		staticShader.setVec3("pointLight[21].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[21].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[21].constant", 1.0f);
		staticShader.setFloat("pointLight[21].linear", 0.009f);
		staticShader.setFloat("pointLight[21].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[22].position", glm::vec3(1230.0f, 830.0f, -345.0f));     //luz lámpara interna casa 2 piso 3
		staticShader.setVec3("pointLight[22].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[22].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[22].constant", 1.0f);
		staticShader.setFloat("pointLight[22].linear", 0.009f);
		staticShader.setFloat("pointLight[22].quadratic", 0.00032f);
		
		staticShader.setVec3("pointLight[23].position", glm::vec3(1640.0f, 810.0f, -90.0f));     //luz lámpara interna casa 2 piso 3
		staticShader.setVec3("pointLight[23].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[23].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[23].constant", 1.0f);
		staticShader.setFloat("pointLight[23].linear", 0.009f);
		staticShader.setFloat("pointLight[23].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[24].position", glm::vec3(1210.0f, 810.0f, 340.0f));     //luz lámpara interna casa 1 piso 3
		staticShader.setVec3("pointLight[24].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[24].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[24].constant", 1.0f);
		staticShader.setFloat("pointLight[24].linear", 0.009f);
		staticShader.setFloat("pointLight[24].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[25].position", glm::vec3(1640.0f, 810.0f, 690.0f));     //luz lámpara interna casa 1 piso 3
		staticShader.setVec3("pointLight[25].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[25].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("pointLight[25].constant", 1.0f);
		staticShader.setFloat("pointLight[25].linear", 0.009f);
		staticShader.setFloat("pointLight[25].quadratic", 0.00032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(-859.0f, 20.0f, -500.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[1].position", glm::vec3 (- 859.0f, 20.0f, 500.0f));
		staticShader.setVec3("spotLight[1].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[1].constant", 1.0f);
		staticShader.setFloat("spotLight[1].linear", 0.0009f);
		staticShader.setFloat("spotLight[1].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[2].position", glm::vec3(859.0f, 20.0f, -500.0f));
		staticShader.setVec3("spotLight[2].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[2].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[2].constant", 1.0f);
		staticShader.setFloat("spotLight[2].linear", 0.0009f);
		staticShader.setFloat("spotLight[2].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[3].position", glm::vec3(859.0f, 20.0f, 800.0f));
		staticShader.setVec3("spotLight[3].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[3].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[3].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[3].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[3].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[3].constant", 1.0f);
		staticShader.setFloat("spotLight[3].linear", 0.0009f);
		staticShader.setFloat("spotLight[3].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[4].position", glm::vec3(859.0f, 20.0f, -2100.0f));
		staticShader.setVec3("spotLight[4].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[4].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[4].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[4].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[4].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[4].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[4].constant", 1.0f);
		staticShader.setFloat("spotLight[4].linear", 0.0009f);
		staticShader.setFloat("spotLight[4].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[5].position", glm::vec3(600.0f, 20.0f, 1159.0f));
		staticShader.setVec3("spotLight[5].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[5].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[5].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[5].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[5].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[5].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[5].constant", 1.0f);
		staticShader.setFloat("spotLight[5].linear", 0.0009f);
		staticShader.setFloat("spotLight[5].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[6].position", glm::vec3(-600.0f, 20.0f, 1159.0f));
		staticShader.setVec3("spotLight[6].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[6].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[6].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[6].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[6].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[6].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[6].constant", 1.0f);
		staticShader.setFloat("spotLight[6].linear", 0.0009f);
		staticShader.setFloat("spotLight[6].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[7].position", glm::vec3(1391.0f, 20.0f, 2150.0f));
		staticShader.setVec3("spotLight[7].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[7].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[7].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[7].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[7].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[7].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[7].constant", 1.0f);
		staticShader.setFloat("spotLight[7].linear", 0.0009f);
		staticShader.setFloat("spotLight[7].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[8].position", glm::vec3(-1391.0f, 20.0f, 2150.0f));
		staticShader.setVec3("spotLight[8].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[8].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[8].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[8].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[8].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[8].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[8].constant", 1.0f);
		staticShader.setFloat("spotLight[8].linear", 0.0009f);
		staticShader.setFloat("spotLight[8].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[9].position", glm::vec3(600.0f, 20.0f, -1159.0f));
		staticShader.setVec3("spotLight[9].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[9].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[9].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[9].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[9].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[9].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[9].constant", 1.0f);
		staticShader.setFloat("spotLight[9].linear", 0.0009f);
		staticShader.setFloat("spotLight[9].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[10].position", glm::vec3(-600.0f, 20.0f, -1159.0f));
		staticShader.setVec3("spotLight[10].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[10].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[10].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[10].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[10].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[10].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[10].constant", 1.0f);
		staticShader.setFloat("spotLight[10].linear", 0.0009f);
		staticShader.setFloat("spotLight[10].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[11].position", glm::vec3(-1391.0f, 20.0f, -2150.0f));
		staticShader.setVec3("spotLight[11].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[11].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[11].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[11].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[11].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[11].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[11].constant", 1.0f);
		staticShader.setFloat("spotLight[11].linear", 0.0009f);
		staticShader.setFloat("spotLight[11].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[12].position", glm::vec3(-1440.0f, 290.0f, -330.0f));
		staticShader.setVec3("spotLight[12].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[12].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[12].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[12].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[12].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[12].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[12].constant", 1.0f);
		staticShader.setFloat("spotLight[12].linear", 0.0009f);
		staticShader.setFloat("spotLight[12].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[13].position", glm::vec3(-1440.0f, 290.0f, 210.0f));
		staticShader.setVec3("spotLight[13].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[13].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[13].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[13].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[13].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[13].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[13].constant", 1.0f);
		staticShader.setFloat("spotLight[13].linear", 0.0009f);
		staticShader.setFloat("spotLight[13].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[14].position", glm::vec3(1640.0f, 900.0f, 690.0f));
		staticShader.setVec3("spotLight[14].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[14].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[14].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[14].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[14].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[14].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[14].constant", 1.0f);
		staticShader.setFloat("spotLight[14].linear", 0.0009f);
		staticShader.setFloat("spotLight[14].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[15].position", glm::vec3(1210.0f, 900.0f, 340.0f));
		staticShader.setVec3("spotLight[15].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[15].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[15].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[15].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[15].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[15].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[15].constant", 1.0f);
		staticShader.setFloat("spotLight[15].linear", 0.0009f);
		staticShader.setFloat("spotLight[15].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[16].position", glm::vec3(1210.0f, 900.0f, 340.0f));
		staticShader.setVec3("spotLight[16].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[16].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[16].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[16].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[16].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[16].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[16].constant", 1.0f);
		staticShader.setFloat("spotLight[16].linear", 0.0009f);
		staticShader.setFloat("spotLight[16].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[17].position", glm::vec3(1230.0f, 900.0f, -345.0f));
		staticShader.setVec3("spotLight[17].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[17].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[17].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[17].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[17].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[17].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[17].constant", 1.0f);
		staticShader.setFloat("spotLight[17].linear", 0.0009f);
		staticShader.setFloat("spotLight[17].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[18].position", glm::vec3(1730.0f, 580.0f, 420.0f));     //luz lámpara interna casa 1 piso 2
		staticShader.setVec3("spotLight[18].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[18].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[18].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[18].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[18].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[18].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[18].constant", 1.0f);
		staticShader.setFloat("spotLight[18].linear", 0.0009f);
		staticShader.setFloat("spotLight[18].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[19].position", glm::vec3(1685.0f, 580.0f, -330.0f));     //luz lámpara interna casa 2 piso 2
		staticShader.setVec3("spotLight[19].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[19].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[19].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[19].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[19].cutOff", glm::cos(glm::radians(1000000.0f)));
		staticShader.setFloat("spotLight[19].outerCutOff", glm::cos(glm::radians(600000.0f)));
		staticShader.setFloat("spotLight[19].constant", 1.0f);
		staticShader.setFloat("spotLight[19].linear", 0.0009f);
		staticShader.setFloat("spotLight[19].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[20].position", glm::vec3(1270.0f, 230.0f, 790.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("spotLight[20].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[20].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[20].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[20].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[20].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[20].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[20].constant", 1.0f);
		staticShader.setFloat("spotLight[20].linear", 0.0009f);
		staticShader.setFloat("spotLight[20].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[21].position", glm::vec3(1689.0f, 230.0f, 790.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("spotLight[21].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[21].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[21].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[21].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[21].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[21].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[21].constant", 1.0f);
		staticShader.setFloat("spotLight[21].linear", 0.0009f);
		staticShader.setFloat("spotLight[21].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[22].position", glm::vec3(1285.0f, 230.0f, 290.0f));     //luz lámpara interna casa 1 piso 1
		staticShader.setVec3("spotLight[22].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[22].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[22].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[22].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[22].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[22].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[22].constant", 1.0f);
		staticShader.setFloat("spotLight[22].linear", 0.0009f);
		staticShader.setFloat("spotLight[22].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[23].position", glm::vec3(1260.0f, 230.0f, -4.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("spotLight[23].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[23].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[23].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[23].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[23].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[23].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[23].constant", 1.0f);
		staticShader.setFloat("spotLight[23].linear", 0.0009f);
		staticShader.setFloat("spotLight[23].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[24].position", glm::vec3(1660.0f, 230.0f, -4.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("spotLight[24].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[24].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[24].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[24].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[24].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[24].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[24].constant", 1.0f);
		staticShader.setFloat("spotLight[24].linear", 0.0009f);
		staticShader.setFloat("spotLight[24].quadratic", 0.0005f);

		staticShader.setVec3("spotLight[25].position", glm::vec3(1280.0f, 230.0f, -450.0f));     //luz lámpara interna casa 2 piso 1
		staticShader.setVec3("spotLight[25].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[25].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[25].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("spotLight[25].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setFloat("spotLight[25].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[25].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[25].constant", 1.0f);
		staticShader.setFloat("spotLight[25].linear", 0.0009f);
		staticShader.setFloat("spotLight[25].quadratic", 0.0005f);

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
		model = glm::scale(model, glm::vec3(0.75));
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
		model = glm::scale(model, glm::vec3(0.85f));
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1480.0f, 0.0f, 1250.0f)); 		//árbol alrededor (zona 1)
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		staticShader.setMat4("model", model);
		ArbolVerde.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1720.0f, 0.0f, 1750.0f));			//árbol alrededor (zona 1)
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + movAuto_x, -30.0f, -1060.0f + movAuto_z));
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

		/************************************** BANCAS *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(316.0f, 0.0f, 406.0f));
		model = glm::rotate(model, glm::radians(142.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(312.0f, 0.0f, -432.0f));
		model = glm::rotate(model, glm::radians(216.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-330.0f, 0.0f, -420.0f));
		model = glm::rotate(model, glm::radians(322.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-308.0f, 0.0f, 428.0f));
		model = glm::rotate(model, glm::radians(396.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

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

		/*************************************** Mesa Exterior *************************************/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, 10.0f, 3600.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		mesaExterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 3600.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		mesaExterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 10.0f, 3600.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		mesaExterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 10.0f, 2200.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		mesaExterior.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-600.0f, 10.0f, 2200.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		mesaExterior.Draw(staticShader);

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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, 2000.0f));
		model = glm::scale(model, glm::vec3(11.0f));
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
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, -600.0f));
		model = glm::scale(model, glm::vec3(2.1f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		gallery.Draw(staticShader);



		/************************************** Houses *************************************/

		//House 1
		model = glm::translate(model, glm::vec3(-78.0f, 0.0f, 438.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 2
		model = glm::translate(model, glm::vec3(49.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 3
		model = glm::translate(model, glm::vec3(49.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		house.Draw(staticShader);

		//House 4
		model = glm::translate(model, glm::vec3(49.0f, 0.0f, 0.0f));
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

		//----------------------------------- door Gallery ------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1000.0f, 2.0f, -78.0f));
		model = glm::scale(model, glm::vec3(7.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		doorGallery.Draw(staticShader);

		//----------------------------------- door House ------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1145.0f, 105.0f, -1811.5f));
		model = glm::scale(model, glm::vec3(29.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		doorHouse.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1145.0f, 115.0f, -232.0f));
		model = glm::scale(model, glm::vec3(27.0f));
		staticShader.setMat4("model", model);
		doorHouse.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1145.0f, 112.0f, -1025.0f));
		model = glm::scale(model, glm::vec3(27.0f));
		staticShader.setMat4("model", model);
		doorHouse.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(1145.0f, 112.0f, 559.0f));
		model = glm::scale(model, glm::vec3(27.0f));
		staticShader.setMat4("model", model);
		doorHouse.Draw(staticShader);

		
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
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
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
	//Toggle Walking Man Animation
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		turnOnWalkMan ^= true;

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