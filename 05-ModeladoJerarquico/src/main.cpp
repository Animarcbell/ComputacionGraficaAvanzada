//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenWidth;
int screenHeight;
//Variambres de rotación
float rotx = 0.0f;
float roty = 0.0f;
float rotz = 0.0f;
//Variables para el desplazamiento
float despx = 0.0f;
float despy= 0.0f;
float despz = 0.0f;
//Variables articulaciones
float brazosz = 0.25;
float piernax = 0.0;
float rodix = 0.0;
bool movimiento = false;
bool caminata = false;


GLFWwindow * window;

Shader shader;
//Variables de las figuras
Box Esponja;
Box Camisa;
Box Pantalon;
Sphere Manga(10, 10);
Cylinder Brazo(8, 8, 0.5, 0.3);
Sphere Artic(10, 10);
Sphere Pierna(10, 10);
Cylinder Calceta(8, 8, 0.5, 0.3);
Sphere Extrem(10, 10);
Box Dientes;
Box BGorro;
Sphere Ojoazul(20, 20);
Box Corbata;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	// Inicializar las figuras

	Esponja.init();
	Esponja.setShader(&shader);
	Esponja.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	Camisa.init();
	Camisa.setShader(&shader);
	Camisa.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	Pantalon.init();
	Pantalon.setShader(&shader);
	Pantalon.setColor(glm::vec4(0.6, 0.4, 0.2, 1.0));

	Manga.init();
	Manga.setShader(&shader);
	Manga.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	Brazo.init();
	Brazo.setShader(&shader);
	Brazo.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	Artic.init();
	Artic.setShader(&shader);
	Artic.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	Pierna.init();
	Pierna.setShader(&shader);
	Pierna.setColor(glm::vec4(0.6, 0.4, 0.2, 1.0));

	Calceta.init();
	Calceta.setShader(&shader);
	Calceta.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	Extrem.init();
	Extrem.setShader(&shader);
	Extrem.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

	Dientes.init();
	Dientes.setShader(&shader);
	Dientes.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	BGorro.init();
	BGorro.setShader(&shader);
	BGorro.setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

	Ojoazul.init();
	Ojoazul.setShader(&shader);
	Ojoazul.setColor(glm::vec4(0.0, 0.8, 1.0, 1.0));

	Corbata.init();
	Corbata.setShader(&shader);
	Corbata.setColor(glm::vec4(1.0, 0.0, 0.0, 1.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	Esponja.destroy();
	Camisa.destroy();
	Pantalon.destroy();
	Manga.destroy();
	Brazo.destroy();
	Artic.destroy();
	Pierna.destroy();
	Calceta.destroy();
	Extrem.destroy();
	Dientes.destroy();
	BGorro.destroy();
	Ojoazul.destroy();
	Corbata.destroy();

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	//Movimiento
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
			movimiento = true;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		movimiento = false;
	}
	//Desplazamiento en X rot en Y
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		despx-=0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		despx += 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		roty -= 0.2;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		roty += 0.2;
	}
	//Desplazamiento en Z rotacion en X
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		despz -= 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		despz += 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		rotx -= 0.2;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		rotx += 0.2;
	}
	//Desplazamiento en Y rotacion en Z
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		despy -= 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		despy += 0.005;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		rotz -= 0.2;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rotz += 0.2;
	}



	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		if (movimiento) {
			if (brazosz < 1.5) {
				brazosz += 0.1;
			}
			if (rodix < 1.5) {
				rodix += 0.1;
			}
			if (piernax < 1.5 && !caminata) {
				piernax += 0.05;
			}
			if (piernax >= 1.5) {
				caminata = true;
			}
			if (piernax > -1.5 && caminata) {
				piernax -= 0.05;
			}
			if (piernax <= -1.5) {
				caminata = false;
			}
		}

		if (!movimiento) {
			if (brazosz > 0.25) {
				brazosz -= 0.1;
			}
			if (rodix > 0) {
				rodix -= 0.1;
			}
			if (piernax < 0) {
				piernax += 0.05;
			}
			if (piernax > 0) {
				piernax -= 0.05;
			}
			if (piernax <= 0) {
				caminata = false;
			}
		}
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.01f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);

		//Esponja.enableWireMode();
		model = glm::translate(model, glm::vec3(despx, despy, despz));
		model = glm::rotate(model, glm::radians(rotx), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(roty), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(rotz), glm::vec3(0.0, 0.0, 1.0));
		Esponja.render(glm::scale(model, glm::vec3(0.7, 0.7, 0.2)));
			// Camisa
			glm::mat4 Cami = glm::translate(model, glm::vec3(0.0, -0.4, 0.0));
			Camisa.render(glm::scale(Cami, glm::vec3(0.7, 0.1, 0.2)));
				//Corbata
				glm::mat4 Corb = glm::translate(Cami, glm::vec3(0.0, 0.025, 0.1));
				Corbata.render(glm::scale(Corb, glm::vec3(0.06, 0.05, 0.025)));
					glm::mat4 CorbB = glm::translate(Corb, glm::vec3(0.0, -0.075, 0.0));
					Corbata.render(glm::scale(CorbB, glm::vec3(0.05, 0.1, 0.025)));
				//Brazo Derecho
				// Manga
				glm::mat4 Man1 = glm::translate(Cami, glm::vec3(0.35f, 0.0f, 0.0f));
				Man1 = glm::rotate(Man1, brazosz, glm::vec3(0.0, 0.0, 1.0));
				Manga.render(glm::scale(Man1, glm::vec3(0.1, 0.1, 0.1)));
					// Brazo
					glm::mat4 BraD = glm::translate(Man1, glm::vec3(0.0, -0.1, 0.0));
					Brazo.render(glm::scale(BraD, glm::vec3(0.1, 0.2, 0.1)));
						// Codo
						glm::mat4 CodD = glm::translate(BraD, glm::vec3(0.0, -0.1, 0.0));
						CodD = glm::rotate(CodD, -brazosz, glm::vec3(0.0, 0.0, 1.0));
						Artic.render(glm::scale(CodD, glm::vec3(0.075, 0.075, 0.075)));
							// Antebrazo
							glm::mat4 AnBD = glm::translate(CodD, glm::vec3(0.0, -0.1, 0.0));
							Brazo.render(glm::scale(AnBD, glm::vec3(0.075, 0.2, 0.075)));
								// Mano
								glm::mat4 ManoD = glm::translate(AnBD, glm::vec3(0.0, -0.1, 0.0));
								Artic.render(glm::scale(ManoD, glm::vec3(0.1, 0.1, 0.1)));
				//Brazo Izquierdo
				// Manga
				glm::mat4 Man2 = glm::translate(Cami, glm::vec3(-0.35f, 0.0f, 0.0f));
				Man2 = glm::rotate(Man2, -brazosz, glm::vec3(0.0, 0.0, 1.0));
				Manga.render(glm::scale(Man2, glm::vec3(0.1, 0.1, 0.1)));
					// Brazo
					glm::mat4 BraI = glm::translate(Man2, glm::vec3(0.0, -0.1, 0.0));
					Brazo.render(glm::scale(BraI, glm::vec3(0.1, 0.2, 0.1)));
						// Codo
						glm::mat4 CodI = glm::translate(BraI, glm::vec3(0.0, -0.1, 0.0));
						CodI = glm::rotate(CodI, brazosz, glm::vec3(0.0, 0.0, 1.0));
						Artic.render(glm::scale(CodI, glm::vec3(0.075, 0.075, 0.075)));
							// AnteBrazo
							glm::mat4 AnBI = glm::translate(CodI, glm::vec3(0.0, -0.1, 0.0));
							Brazo.render(glm::scale(AnBI, glm::vec3(0.075, 0.2, 0.075)));
								// Mano
								glm::mat4 ManoI = glm::translate(AnBI, glm::vec3(0.0, -0.1, 0.0));
								Artic.render(glm::scale(ManoI, glm::vec3(0.1, 0.1, 0.1)));
			// Pantalon
			glm::mat4 Pant = glm::translate(model, glm::vec3(0.0, -0.5, 0.0));
			Pantalon.render(glm::scale(Pant, glm::vec3(0.7, 0.1, 0.2)));
				// Pierna Derecha
				// Pantalon
				glm::mat4 PiernaD = glm::translate(Pant, glm::vec3(0.175f, -0.075f, 0.0f));
				PiernaD = glm::rotate(PiernaD, piernax, glm::vec3(1.0, 0.0, 0.0));
				Pierna.render(glm::scale(PiernaD, glm::vec3(0.1, 0.1, 0.1)));
					// Pierna
					glm::mat4 PiernD = glm::translate(PiernaD, glm::vec3(0.0, -0.1, 0.0));
					Brazo.render(glm::scale(PiernD, glm::vec3(0.1, 0.2, 0.1)));
						// Rodilla
						glm::mat4 RodD = glm::translate(PiernD, glm::vec3(0.0, -0.1, 0.0));
						RodD = glm::rotate(RodD, rodix, glm::vec3(1.0, 0.0, 0.0));
						Artic.render(glm::scale(RodD, glm::vec3(0.075, 0.075, 0.075)));
							// Calceta
							glm::mat4 CalcetD = glm::translate(RodD, glm::vec3(0.0, -0.1, 0.0));
							Calceta.render(glm::scale(CalcetD, glm::vec3(0.075, 0.2, 0.075)));
								// Pie Derecho
								glm::mat4 TalonD = glm::translate(CalcetD, glm::vec3(0.0, -0.1, -0.02));
								Extrem.render(glm::scale(TalonD, glm::vec3(0.1, 0.1, 0.1)));
									glm::mat4 PuntaD = glm::translate(TalonD, glm::vec3(0.0, 0.0, 0.1));
									Extrem.render(glm::scale(PuntaD, glm::vec3(0.15, 0.15, 0.15)));
				// Pierna Izquierda
				// Pantalon
				glm::mat4 PiernaI = glm::translate(Pant, glm::vec3(-0.175f, -0.075f, 0.0f));
				PiernaI = glm::rotate(PiernaI, piernax, glm::vec3(-1.0, 0.0, 0.0));
				Pierna.render(glm::scale(PiernaI, glm::vec3(0.1, 0.1, 0.1)));
					// Pierna
					glm::mat4 PiernI = glm::translate(PiernaI, glm::vec3(0.0, -0.1, 0.0));
					Brazo.render(glm::scale(PiernI, glm::vec3(0.1, 0.2, 0.1)));
						// Rodilla
						glm::mat4 RodI = glm::translate(PiernI, glm::vec3(0.0, -0.1, 0.0));
						RodI = glm::rotate(RodI, rodix, glm::vec3(1.0, 0.0, 0.0));
						Artic.render(glm::scale(RodI, glm::vec3(0.075, 0.075, 0.075)));
							// Calceta
							glm::mat4 CalcetI = glm::translate(RodI, glm::vec3(0.0, -0.1, 0.0));
							Calceta.render(glm::scale(CalcetI, glm::vec3(0.075, 0.2, 0.075)));
								// Pie
								glm::mat4 TalonI = glm::translate(CalcetI, glm::vec3(0.0, -0.1, -0.02));
								Extrem.render(glm::scale(TalonI, glm::vec3(0.1, 0.1, 0.1)));
									glm::mat4 PuntaI = glm::translate(TalonI, glm::vec3(0.0, 0.0, 0.1));
									Extrem.render(glm::scale(PuntaI, glm::vec3(0.15, 0.15, 0.15)));

			//Cara
			// Nariz 
			glm::mat4 Nariz = glm::translate(model, glm::vec3(0.0, 0.0, 0.15));
			Nariz = glm::rotate(Nariz, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			Brazo.render(glm::scale(Nariz, glm::vec3(0.1, 0.2, 0.1)));
			glm::mat4 NarP = glm::translate(Nariz, glm::vec3(0.0, 0.1, 0.0));
			Artic.render(glm::scale(NarP, glm::vec3(0.09, 0.09, 0.09)));
			//Boca
			glm::mat4 Labio = glm::translate(model, glm::vec3(0.0, -0.2, 0.115));
			Esponja.render(glm::scale(Labio, glm::vec3(0.4, 0.1, 0.03)));
			//Dientes
			glm::mat4 DienteD = glm::translate(model, glm::vec3(0.05, -0.25, 0.115));
			Dientes.render(glm::scale(DienteD, glm::vec3(0.05, 0.1, 0.02)));
			glm::mat4 DienteI = glm::translate(model, glm::vec3(-0.05, -0.25, 0.115));
			Dientes.render(glm::scale(DienteI, glm::vec3(0.05, 0.1, 0.02)));
			//Ojos
				//Ojo Derecho
				glm::mat4 OjoD = glm::translate(model, glm::vec3(0.15, 0.1, 0.1));
				Manga.render(glm::scale(OjoD, glm::vec3(0.27, 0.27, 0.01)));
					glm::mat4 ColorD = glm::translate(OjoD, glm::vec3(0.0, 0.0, 0.005));
					Ojoazul.render(glm::scale(ColorD, glm::vec3(0.13, 0.13, 0.01)));
						glm::mat4 PupilaD = glm::translate(ColorD, glm::vec3(0.0, 0.0, 0.005));
						Extrem.render(glm::scale(PupilaD, glm::vec3(0.06, 0.06, 0.01)));
				//Ojo Izquierdo
				glm::mat4 OjoI = glm::translate(model, glm::vec3(-0.15, 0.1, 0.1));
				Manga.render(glm::scale(OjoI, glm::vec3(0.27, 0.27, 0.01)));
					glm::mat4 ColorI = glm::translate(OjoI, glm::vec3(0.0, 0.0, 0.005));
					Ojoazul.render(glm::scale(ColorI, glm::vec3(0.13, 0.13, 0.01)));
						glm::mat4 PupilaI = glm::translate(ColorI, glm::vec3(0.0, 0.0, 0.005));
						Extrem.render(glm::scale(PupilaI, glm::vec3(0.06, 0.06, 0.01)));
			//Gorro
			glm::mat4 BaseGorro = glm::translate(model, glm::vec3(0.0, 0.375, 0.05));
			BGorro.render(glm::scale(BaseGorro, glm::vec3(0.1, 0.05, 0.3)));
				glm::mat4 CentroGorro = glm::translate(BaseGorro, glm::vec3(0.0, 0.1, -0.05));
				Calceta.render(glm::scale(CentroGorro, glm::vec3(0.1, 0.15, 0.15)));
					glm::mat4 TopGorro = glm::translate(CentroGorro, glm::vec3(0.0, 0.075, 0.0));
					Manga.render(glm::scale(TopGorro, glm::vec3(0.1, 0.1, 0.15)));

		shader.turnOff();
		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}