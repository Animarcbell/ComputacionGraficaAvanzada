//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader include
#include "Headers/Shader.h"

Shader shader;

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;
int intercambio = 0;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

GLuint VAO, VBO, EBO;
GLuint VAO2, VBO2, EBO2;
GLuint VAO3, VBO3, EBO3;

typedef struct _Vertex {
	glm::vec3 m_Pos;
	glm::vec3 m_Color;
} Vertex;

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
	glEnable(GL_CULL_FACE);

	shader.initialize("../Shaders/transformaciones.vs",
		"../Shaders/transformaciones.fs");

	// Se definen los vertices de la geometria a dibujar
	Vertex vertices[] =
	{
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) }
	};
	Vertex vertices3[] =
	{
		//Posición				Color
		//Parte centro
		{ { 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 0.6f } },//1
		//Parte Superior
		{ { 0.0f, 0.5f, 0.0f },{ 1.0f, 0.0f, 0.1f } },
		{ { 0.1f, 0.5f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { 0.0f, 0.6f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ {-0.1f, 0.5f, 0.0f },{ 1.0f, 0.5f, 0.7f } },//5
		//Parte izquierda
		{ { -0.5f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.1f } },
		{ { -0.5f, 0.1f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { -0.6f, 0.0f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { -0.5f,-0.1f, 0.0f },{ 1.0f, 0.5f, 0.7f } },//9
		//Parte inferior
		{ { 0.0f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.1f } },
		{ {-0.1f, -0.5f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { 0.0f, -0.6f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { 0.1f, -0.5f, 0.0f },{ 1.0f, 0.5f, 0.7f } },//13
		//Parte Derecha
		{ { 0.5f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.1f } },
		{ { 0.5f,-0.1f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { 0.6f, 0.0f, 0.0f },{ 1.0f, 0.5f, 0.7f } },
		{ { 0.5f, 0.1f, 0.0f },{ 1.0f, 0.5f, 0.7f } }//17
	};

	Vertex vertices2[] =
	{
		//Cara frontal						Color: Rojo
		{ glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 0.0f) },//3
		// Cara derecha						Color: Azul
		{ glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f),		glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(0.0f, 0.0f, 1.0f) },//7
		// Cara izquierda					Color: Blanco
		{ glm::vec3(-0.5f, -0.5f, 0.5f),	glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 1.0f, 1.0f) },//11
		// Cara superior					Color violeta
		{ glm::vec3(-0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f),		glm::vec3(1.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(1.0f, 0.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 1.0f) },//15
		// Cara trasera
		{ glm::vec3(0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f),		glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f) }//19
	};

	// Se definen los indices de las conexiones con los vertices.
	GLuint indices[] = {  // Note que se inicia en 0!
		0, 1, 2,
		0, 2, 3,
		1, 4, 5,
		1, 5, 2,
		0, 3, 6,
		0, 6, 7,
		0, 4, 1,
		0, 7, 4,
		3, 2, 5,
		3, 5, 6,
		4, 5, 6,
		4, 6, 7
	};

	GLuint indices2[] =
	{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11,
			12, 13, 14,
			12, 14, 15,
			16, 18, 17,
			16, 19, 18
	};
	GLuint indices3[] = {  // Note que se inicia en 0!
		0,2,1,
		1,2,3,
		1,3,4,
		0,1,4,

		0,6,5,
		5,6,7,
		5,7,8,
		5,8,0,

		0,10,9,
		9,10,11,
		9,11,12,
		9,12,0,

		0,14,13,
		13,14,15,
		13,15,16,
		13,16,0
	};

	size_t bufferSize = sizeof(vertices);
	size_t vertexSize = sizeof(vertices[0]);
	size_t rgbOffset = sizeof(vertices[0].m_Pos);

	std::cout << "Vertices Estrella:" << std::endl;
	std::cout << "bufferSize:" << bufferSize << std::endl;
	std::cout << "vertexSize:" << vertexSize << std::endl;
	std::cout << "rgbOffset:" << rgbOffset << std::endl;

	// Se crea el ID del VAO
	// Se crea el VBO (buffer de datos) asociado al VAO
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	/*
	El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	Esto es de gran utilidad debido a que solo se configura la estructura una vez
	y se puede utilizar en el loop de renderizado
	*/
	glGenVertexArrays(1, &VAO);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glBindVertexArray(VAO);

	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bufferSize = sizeof(vertices2);
	vertexSize = sizeof(vertices2[0]);
	rgbOffset = sizeof(vertices2[0].m_Pos);

	std::cout << "bufferSize:" << bufferSize << std::endl;
	std::cout << "vertexSize:" << vertexSize << std::endl;
	std::cout << "rgbOffset:" << rgbOffset << std::endl;

	// Se crea el ID del VAO
	// Se crea el VBO (buffer de datos) asociado al VAO
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	/*
	 El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	 Esto es de gran utilidad debido a que solo se configura la estructura una vez
	 y se puede utilizar en el loop de renderizado
	 */
	glGenVertexArrays(1, &VAO2);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glBindVertexArray(VAO2);

	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2,
		GL_STATIC_DRAW);

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)rgbOffset);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	bufferSize = sizeof(vertices2);
	vertexSize = sizeof(vertices2[0]);
	rgbOffset = sizeof(vertices2[0].m_Pos);

	std::cout << "bufferSize:" << bufferSize << std::endl;
	std::cout << "vertexSize:" << vertexSize << std::endl;
	std::cout << "rgbOffset:" << rgbOffset << std::endl;

	// Se crea el ID del VAO
	// Se crea el VBO (buffer de datos) asociado al VAO
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	/*
	 El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	 Esto es de gran utilidad debido a que solo se configura la estructura una vez
	 y se puede utilizar en el loop de renderizado
	 */
	glGenVertexArrays(1, &VAO3);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glBindVertexArray(VAO3);

	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices3, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3,
		GL_STATIC_DRAW);

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)rgbOffset);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	glBindVertexArray(VAO);

	glBindVertexArray(VAO2);

	glBindVertexArray(VAO3);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glDeleteBuffers(1, &VBO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glDeleteBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	glDeleteVertexArrays(1, &VAO2);

	glDeleteVertexArrays(1, &VAO3);
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
		case GLFW_KEY_E:
			intercambio = 1;
			break;
		case GLFW_KEY_S:
			intercambio = 0;
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
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	float angleRot = 0.0f;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.02f, 100.0f);

		angleRot += 0.02;

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelParent = glm::mat4(1.0f);
		modelParent = glm::translate(modelParent, glm::vec3(-2.5, 0.6, -7.0));
		//model = glm::rotate(model, glm::radians(angleRot), glm::vec3(0.0, 1.0, 0.0));

		// Esta linea esta comentada debido a que de momento no se usan los shaders
		// glUseProgram(shaderProgramID);
		shader.turnOn();

		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		if (intercambio == 0) {
		// Cubo cuerpo
		model = glm::scale(modelParent, glm::vec3(1.0, 0.75, 0.5));
		model = glm::translate(model, glm::vec3(0.0, 0.16, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		// Se indica el buffer de datos y la estructura de estos utilizando solo el id del VAO
		glBindVertexArray(VAO2);
		// Primitiva de ensamble
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Cubo cuello
		model = glm::translate(modelParent, glm::vec3(0.0, 0.5, 0.0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.1, 0.2, 0.25));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Cubo Cabeza
		model = glm::translate(modelParent, glm::vec3(0.0, 0.8, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Hombro Derecho
		model = modelParent;
		model = glm::translate(model, glm::vec3(-0.75, 0.4, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.2, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Brazo Derecho
		model = modelParent;
		model = glm::translate(model, glm::vec3(-0.8, -0.2, 0.0));
		model = glm::scale(model, glm::vec3(0.25, 1.1, 0.25));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Hombro Izquierdo
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.75, 0.4, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.2, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// Brazo Izquierdo
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.8, -0.2, 0.0));
		model = glm::scale(model, glm::vec3(0.25, 1.1, 0.25));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Espada 1
		model = modelParent;
		model = glm::translate(model, glm::vec3(1.4, -0.8, 0.0));
		model = glm::scale(model, glm::vec3(1.5, 0.125, 0.125));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Espada 2
		model = modelParent;
		model = glm::translate(model, glm::vec3(1.1, -0.8, 0.0));
		model = glm::scale(model, glm::vec3(0.2, 0.4, 0.125));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Torso
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.0, -0.35, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.25, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Pelvis
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.0, -0.66, 0.0));
		model = glm::scale(model, glm::vec3(0.75, 0.4, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Pierna derecha
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.18, -1.48, 0.0));
		model = glm::scale(model, glm::vec3(0.3, 1.2, 0.3));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Pie derecha
		model = modelParent;
		model = glm::translate(model, glm::vec3(0.38, -2.22, 0.0));
		model = glm::scale(model, glm::vec3(0.7, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Pierna Izquierda
		model = modelParent;
		model = glm::translate(model, glm::vec3(-0.18, -1.48, 0.0));
		model = glm::scale(model, glm::vec3(0.3, 1.2, 0.3));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Pie izquierda
		model = modelParent;
		model = glm::translate(model, glm::vec3(-0.38, -2.22, 0.0));
		model = glm::scale(model, glm::vec3(0.7, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Perro
		//Cuerpo1
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.5, -1.5, 0.0));
		model = glm::scale(model, glm::vec3(0.7, 0.7, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Cuerpo2
		model = modelParent;
		model = glm::translate(model, glm::vec3(4.2, -1.6, 0.0));
		model = glm::scale(model, glm::vec3(0.7, 0.5, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Patas1
		model = modelParent;
		model = glm::translate(model, glm::vec3(4.4, -2.1, 0.0));
		model = glm::scale(model, glm::vec3(0.3, 0.55, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Patas1
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.4, -2.1, 0.0));
		model = glm::scale(model, glm::vec3(0.3, 0.55, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Cola
		model = modelParent;
		model = glm::translate(model, glm::vec3(4.8, -1.5, 0.0));
		model = glm::scale(model, glm::vec3(0.7, 0.1, 0.1));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Cabeza
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.6, -0.8, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.7, 0.5));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//orejas
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.7, -0.3, 0.0));
		model = glm::scale(model, glm::vec3(0.2, 0.3, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//trompa
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.3, -0.9, 0.0));
		model = glm::scale(model, glm::vec3(0.25, 0.35, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		model = modelParent;
		model = glm::translate(model, glm::vec3(3.1, -0.84, 0.0));
		model = glm::scale(model, glm::vec3(0.15, 0.24, 0.5));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		}
		else {
		model = glm::scale(modelParent, glm::vec3(5.5, 5.5, 1.0));
		model = glm::translate(model, glm::vec3(0.45, -0.15, 0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindVertexArray(VAO3);
		// Primitiva de ensamble
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		}
		glfwSwapBuffers(window);

	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
