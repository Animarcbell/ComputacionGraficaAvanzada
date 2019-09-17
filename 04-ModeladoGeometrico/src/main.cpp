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
//Rotaciones en ejes
float angRotX = 0.0f;
float angRotY = 0.0f;
float angRotZ = 0.0f;

float moveX = 0.0f;
float moveY = 0.25f;
float moveZ = 0.0f;


GLFWwindow * window;

Shader shader;

Sphere sphere1(20, 20);
Sphere sphereEye(20, 20);
Sphere spherePupile(20, 20);
Cylinder cylinder1(4, 4, 0.5, 0.3);
Cylinder cylinder2(8, 8, 0.5, 0.5);

Box box1;
Box box2;
Box boxTeeth;
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

	sphere1.init();
	sphere1.setShader(&shader);
	sphere1.setColor(glm::vec4(0.3, 1.0, 0.3, 1.0));

	sphereEye.init();
	sphereEye.setShader(&shader);
	sphereEye.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	spherePupile.init();
	spherePupile.setShader(&shader);
	spherePupile.setColor(glm::vec4(0.0, 0.5, 1.0, 1.0));

	cylinder1.init();
	cylinder1.setShader(&shader);
	cylinder1.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	cylinder2.init();
	cylinder2.setShader(&shader);
	cylinder2.setColor(glm::vec4(0.9, 0.9, 0.0, 1.0));

	box1.init();
	box1.setShader(&shader);
	box1.setColor(glm::vec4(1.0, 1.0, 0.0, 1.0));

	box2.init();
	box2.setShader(&shader);
	box2.setColor(glm::vec4(0.9, 0.6, 0.55, 1.0));

	boxTeeth.init();
	boxTeeth.setShader(&shader);
	boxTeeth.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	sphere1.destroy();
	cylinder1.destroy();
	box1.destroy();
	box2.destroy();
	boxTeeth.destroy();
	sphereEye.destroy();
	spherePupile.destroy();

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
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			moveX -= 0.001;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			angRotX += 0.05;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			angRotX -= 0.05;
		}
		else
			moveX += 0.001;

	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			moveY -= 0.001;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			angRotY += 0.05;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			angRotY -= 0.05;
		}
		else
			moveY += 0.001;
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			moveZ -= 0.001;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			angRotZ += 0.05;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			angRotZ -= 0.05;
		}
		else
			moveZ += 0.001;
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.01f, 100.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f);

		//box1.enableWireMode();
		model = glm::translate(model, glm::vec3(moveX, moveY, moveZ));
		model = glm::rotate(model, glm::radians(angRotX), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(angRotY), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(angRotZ), glm::vec3(0.0, 0.0, 1.0));
		box1.render(glm::scale(model, glm::vec3(0.8, 1.0, 0.1)));

		// Articulacion 1
		glm::mat4 j1 = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		j1 = glm::rotate(j1, glm::radians(10.0f), glm::vec3(0.0, 0.0, 1.0));
		j1 = glm::rotate(j1, glm::radians(-20.0f), glm::vec3(0.0, 1.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j1, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 1 izquierda
		glm::mat4 j1i = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		j1i = glm::rotate(j1i, glm::radians(-10.0f), glm::vec3(0.0, 0.0, 1.0));
		j1i = glm::rotate(j1i, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j1i, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 1
		glm::mat4 l1 = glm::translate(j1, glm::vec3(0.15, 0.0, 0.0));
		l1 = glm::rotate(l1, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		l1 = glm::scale(l1, glm::vec3(0.1, 0.25, 0.1));
		//cylinder1.enableWireMode();
		cylinder1.render(l1);

		// Hueso 1 izquierdo
		glm::mat4 l1i = glm::translate(j1i, glm::vec3(-0.15, 0.0, 0.0));
		l1i = glm::rotate(l1i, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		l1i = glm::scale(l1i, glm::vec3(0.1, 0.25, 0.1));
		//cylinder1.enableWireMode();
		cylinder1.render(l1i);

		// Articulacion 2
		glm::mat4 j2 = glm::translate(j1, glm::vec3(0.25, 0.0, 0.0));
		j2 = glm::rotate(j2, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j2, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 2 izquierda
		glm::mat4 j2i = glm::translate(j1i, glm::vec3(-0.25, 0.0, 0.0));
		j2i = glm::rotate(j2i, glm::radians(120.0f), glm::vec3(0.0, 0.0, 1.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j2i, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso 2
		glm::mat4 l2 = glm::translate(j2, glm::vec3(0.25, 0.0, 0.0));
		l2 = glm::rotate(l2, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		l2 = glm::scale(l2, glm::vec3(0.1, 0.5, 0.1));
		/*cylinder1.enableWireMode();*/
		cylinder1.render(l2);

		// Hueso 2 izquierdo
		glm::mat4 l2i = glm::translate(j2i, glm::vec3(-0.25, 0.0, 0.0));
		l2i = glm::rotate(l2i, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		l2i = glm::scale(l2i, glm::vec3(0.1, 0.5, 0.1));
		/*cylinder1.enableWireMode();*/
		cylinder1.render(l2i);

		// Articulacion 3
		glm::mat4 j3 = glm::translate(j2, glm::vec3(0.5, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j3, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 3 izquierda
		glm::mat4 j3i = glm::translate(j2i, glm::vec3(-0.5, 0.0, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j3i, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 4 (Pierna)
		glm::mat4 j4 = glm::translate(model, glm::vec3(0.25, -0.6, 0.0));
		j4 = glm::rotate(j4, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		j4 = glm::rotate(j4, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j4, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 4 (Pierna) izquierda
		glm::mat4 j4i = glm::translate(model, glm::vec3(-0.25, -0.6, 0.0));
		/*j4i = glm::rotate(j4i, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));*/
		j4i = glm::rotate(j4i, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j4i, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso (Pierna)
		glm::mat4 l3 = glm::translate(j4, glm::vec3(0.0f, -0.10f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l3, glm::vec3(0.1, 0.2, 0.1)));

		// Hueso (Pierna) izquierda
		glm::mat4 l3i = glm::translate(j4i, glm::vec3(0.0f, -0.10f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l3i, glm::vec3(0.1, 0.2, 0.1)));

		// Articulacion 4 (Chamorro)
		glm::mat4 j5 = glm::translate(l3, glm::vec3(0.0, -0.10, 0.0));
		j5 = glm::rotate(j5, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j5, glm::vec3(0.1, 0.1, 0.1)));

		// Articulacion 4 (Chamorro) izquierdo
		glm::mat4 j5i = glm::translate(l3i, glm::vec3(0.0, -0.10, 0.0));
		//sphere1.enableWireMode();
		sphere1.render(glm::scale(j5i, glm::vec3(0.1, 0.1, 0.1)));

		// Hueso (Chamorro)
		glm::mat4 l4 = glm::translate(j5, glm::vec3(0.0f, -0.1f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l4, glm::vec3(0.1, 0.2, 0.1)));

		// Hueso (Pie)
		glm::mat4 hPie = glm::translate(l4, glm::vec3(0.0f, -0.15f, 0.1f));
		box2.render(glm::scale(hPie, glm::vec3(0.1, 0.1, 0.25)));

		// Hueso (Chamorro) izquierdo
		glm::mat4 l4i = glm::translate(j5i, glm::vec3(0.0f, -0.1f, 0.0f));
		//cylinder1.enableWireMode();
		cylinder1.render(glm::scale(l4i, glm::vec3(0.1, 0.2, 0.1)));

		// Hueso (Pie) izquierdo
		glm::mat4 hPiei = glm::translate(l4i, glm::vec3(0.0f, -0.15f, 0.1f));
		box2.render(glm::scale(hPiei, glm::vec3(0.1, 0.1, 0.25)));

		// Pantalon
		glm::mat4 panto = glm::translate(model, glm::vec3(0.0, -0.55, 0.0));
		box2.render(glm::scale(panto, glm::vec3(0.8, 0.1, 0.1)));
		//box1.enableWireMode();

		// Nariz 
		glm::mat4 nariz = glm::translate(model, glm::vec3(0.0, 0.0, 0.2));
		nariz = glm::rotate(nariz, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		nariz = glm::scale(nariz, glm::vec3(0.1, 0.4, 0.1));
		//cylinder2.enableWireMode();
		cylinder2.render(nariz);

		// Boca
		glm::mat4 l5 = glm::translate(model, glm::vec3(0.0, -0.2, 0.05));
		box2.render(glm::scale(l5, glm::vec3(0.6, 0.1, 0.05)));
		//box1.enableWireMode();

		// Dientes
		glm::mat4 Diente1 = glm::translate(model, glm::vec3(0.055, -0.25, 0.05));
		boxTeeth.render(glm::scale(Diente1, glm::vec3(0.1, 0.2, 0.075)));
		glm::mat4 Diente2 = glm::translate(model, glm::vec3(-0.055, -0.25, 0.05));
		boxTeeth.render(glm::scale(Diente2, glm::vec3(0.1, 0.2, 0.075)));
		// Ojo Derecho
		glm::mat4 l6 = glm::translate(model, glm::vec3(0.15, 0.20, 0.05));
		sphereEye.render(glm::scale(l6, glm::vec3(0.2, 0.3, 0.05)));

		// Pupila Derecha
		glm::mat4 l6p = glm::translate(l6, glm::vec3(0.0, -0.05, 0.02));
		spherePupile.render(glm::scale(l6p, glm::vec3(0.1, 0.15, 0.05)));

		// Ojo Izquierdo
		glm::mat4 l6i = glm::translate(model, glm::vec3(-0.15, 0.20, 0.05));
		sphereEye.render(glm::scale(l6i, glm::vec3(0.2, 0.3, 0.05)));

		// Pupila Izquierda
		glm::mat4 l6pi = glm::translate(l6i, glm::vec3(0.0, -0.05, 0.02));
		spherePupile.render(glm::scale(l6pi, glm::vec3(0.1, 0.15, 0.05)));


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