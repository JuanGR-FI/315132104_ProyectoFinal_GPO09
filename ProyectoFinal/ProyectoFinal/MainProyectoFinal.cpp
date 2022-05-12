#include <iostream>
#include <cmath>


// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipo de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacionPelota();
void animacionGarage();
void animacionPerro();


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
//Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
Camera  camera(glm::vec3(7.0f, 15.0f, 70.0f));

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

//Variables de posiciones y rotaciones para animaciones
float rotPuertaGarage = 0.0f;
float posPuertaGarage = 0.0f;
bool animGarage = false;
float posCoche = 0.0f;
float rotPuertaPrincipal = 0.0f;
bool animPuerta = false;
float posPelota = 0.0f;
bool animPelota = false;
float escalaPelota = 1.0f;
float rotVentilador = 0.0f;
bool animVentilador = false;
float posPerro = 0.0f;
float rotPerro = 0.0f;
bool animPerro = false;

//Variables de estado para las animaciones
bool estadoPelota1 = true;
bool estadoPelota2 = false;
bool estadoPelota3 = false;
bool estadoPelota4 = false;

bool estadoGarage1 = true;
bool estadoGarage2 = false;
bool estadoGarage3 = false;
bool estadoGarage4 = false;
bool estadoGarage5 = false;

bool estadoPerro1 = true;
bool estadoPerro2 = false;
bool estadoPerro3 = false;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, -8.0f),
	glm::vec3(8.0f,0.0f,  -8.0f),
	glm::vec3(8.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);

glm::vec3 LightPosition = glm::vec3(4.0, 4.0, -4.0);
glm::vec3 LightDirection = glm::vec3(0.0, -1.0, 0.0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Piso((char*)"Models/Esfera/Piso.obj");
	Model Esfera((char*)"Models/Esfera/Esfera.obj");

	Model Cajonera((char*)"Models/Cajonera/Cajonera.obj");
	Model Table((char*)"Models/Table/table2.obj");
	Model Ball((char*)"Models/Ball/pixarBall.obj");
	Model Basket((char*)"Models/Basket/basket.obj");
	Model BookCase((char*)"Models/Libros/BookCase.obj");
	Model Caballete((char*)"Models/Caballete/Caballete.obj");
	Model Silla((char*)"Models/Silla/Silla.obj");
	Model Cama((char*)"Models/Cama/Cama.obj");
	Model Fachada((char*)"Models/Fachada/Fachada.obj");
	Model PuertaPrincipal((char*)"Models/Fachada/PuertaPrincipal.obj");
	Model PuertaGarage((char*)"Models/Fachada/PuertaGarage.obj");
	Model BaseVentilador((char*)"Models/Ventilador/BaseVentilador.obj");
	Model Ventilador((char*)"Models/Ventilador/Ventilador.obj");
	Model Carroseria((char*)"Models/Car/Carroseria.obj");
	Model LLanta((char*)"Models/Car/Wheel.obj");
	Model Perro((char*)"Models/Dog/Doguinho.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacionPelota();
		animacionGarage();
		animacionPerro();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		//Load Model

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		// Point light 2
		glm::vec3 lightColor2;
		lightColor2.x = abs(sin(glfwGetTime() * Light2.x));
		lightColor2.y = abs(sin(glfwGetTime() * Light2.y));
		lightColor2.z = sin(glfwGetTime() * Light2.z);

		// Point light 3
		glm::vec3 lightColor3;
		lightColor3.x = abs(sin(glfwGetTime() * Light3.x));
		lightColor3.y = abs(sin(glfwGetTime() * Light3.y));
		lightColor3.z = sin(glfwGetTime() * Light3.z);

		// Point light 4
		glm::vec3 lightColor4;
		lightColor4.x = abs(sin(glfwGetTime() * Light4.x));
		lightColor4.y = abs(sin(glfwGetTime() * Light4.y));
		lightColor4.z = sin(glfwGetTime() * Light4.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.20f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.20f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lightColor4.x, lightColor4.y, lightColor4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.22f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.20f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), LightPosition.x, LightPosition.y, LightPosition.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), LightDirection.x, LightDirection.y, LightDirection.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		//Carga de modelo de Piso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso.Draw(lightingShader);

		//Carga de modelo de Mesa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 2.0f));
		model = glm::scale(model,glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Table.Draw(lightingShader);

		//Carga de modelo de Pelota
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(12.0f, 1.5f, 7.0f));
		model = glm::translate(model, glm::vec3(0.0f, posPelota , 0.0f));
		model = glm::scale(model,glm::vec3(1.0f,escalaPelota,1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Ball.Draw(lightingShader);

		//Carga de modelo de Canasta de Juguetes
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 0.5f, -7.0f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Basket.Draw(lightingShader);

		//Carga de modelo de Cajonera
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 13.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cajonera.Draw(lightingShader);

		//Carga de modelo de Librero
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(21.0f, 0.5f, -4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		BookCase.Draw(lightingShader);

		//Carga de modelo de Caballete
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(17.0f, 0.0f, 5.0f));
		model = glm::rotate(model, glm::radians(210.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Caballete.Draw(lightingShader);

		//Carga de modelo de Silla 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla.Draw(lightingShader);

		//Carga de modelo de Silla 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.5f, 0.5f, 3.5f));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla.Draw(lightingShader);

		//Carga de modelo de Cama
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(10.0f, 0.5f, -8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama.Draw(lightingShader);

		//Carga de modelo de Fachada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Fachada.Draw(lightingShader);

		//Carga de modelo de PuertaPrincipal
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.145f, 3.8f, 15.0f));
		model = glm::rotate(model, glm::radians(-rotPuertaPrincipal), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaPrincipal.Draw(lightingShader);

		//Carga de modelo de PuertaGarage
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.0f, 9.0f, 7.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -posPuertaGarage));
		model = glm::rotate(model, glm::radians(-rotPuertaGarage), glm::vec3(1.0f,0.0f,0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaGarage.Draw(lightingShader);

		//Carga de modelo de BaseVentilador
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 15.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		BaseVentilador.Draw(lightingShader);

		//Carga de modelo de Ventilador
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 15.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotVentilador), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Ventilador.Draw(lightingShader);

		//Carroceria
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 23.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -posCoche));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.75);
		Carroseria.Draw(lightingShader);

		//Llanta Delantera Der
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-19.5f, 1.0f, 26.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posCoche));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LLanta.Draw(lightingShader);

		//Llanta Trasera Der
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-19.5f, 1.0f, 18.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posCoche));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LLanta.Draw(lightingShader);


		//Llanta Delantera Izq
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.5f, 1.0f, 26.9f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -posCoche));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LLanta.Draw(lightingShader);

		//Llanta Trasera Izq
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-14.5f, 1.0f, 18.7f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -posCoche));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LLanta.Draw(lightingShader);

		//Carga de modelo de Perro
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, posPerro));
		model = glm::rotate(model, glm::radians(-rotPerro), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Perro.Draw(lightingShader);




		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		



		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Mueve la posición de la cámara y las animaciones dependiendo de las entradas del usuario
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime+0.05);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime+0.05);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime+0.05);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime+0.05);


	}

	if (animPuerta) {
		if (rotPuertaPrincipal < 90) {
			rotPuertaPrincipal += 0.5f;
		}
	}
	
	if (keys[GLFW_KEY_B]) {
		animPelota = true;
	}

	if (keys[GLFW_KEY_G]) {
		animGarage = true;
	}

	if (keys[GLFW_KEY_T]) {
		animPerro = true;
	}

	if (animVentilador) {
		rotVentilador += 1.0f;
		if (rotVentilador > 360) {
			rotVentilador = 0.0f;
			animVentilador = false;
		}		
	}
}

//La función realiza la lógica de la animación de la pelota
void animacionPelota() {
	if (animPelota) {//Si se activa la animación
		if (estadoPelota1) {//Estado1 Mover pelota hacia arriba
			posPelota += 0.1f;
			if (posPelota >  6) {//Cambiar de estado
				estadoPelota1 = false;
				estadoPelota2 = true;
			}
			
		}
		if (estadoPelota2) {//Estado2 Mover pelota hacia abajo
			posPelota -= 0.2f;
			if (posPelota < 0) {//Cambiar de estado
				estadoPelota2 = false;
				estadoPelota3 = true;
			}
		
		}
		if (estadoPelota3) {//Estado3 Escalar lapelota en el eje Y para que parezca más pequeña
			escalaPelota -= 0.1f;
			if (escalaPelota < 0.2 ) {//Cambio de estado
				estadoPelota3 = false;
				estadoPelota4 = true;
			}
		}
		if (estadoPelota4) {//Estado4 Mover pelota hacia arriba
			posPelota += 0.1f;
			if (escalaPelota < 1.0) {//Escalar la pelota a la escala original
				escalaPelota += 0.1f;

			}
			if (posPelota > 6) {//Cambiar de estado
				estadoPelota4 = false;
				estadoPelota2 = true;
			}
		}

	}
}

//La función realiza la lógica de la animación del garage
void animacionGarage() {
	if (animGarage) {//Si se activa la animación
		if (estadoGarage1) {//Estado1 Rotar puerta
			rotPuertaGarage += 0.3f;
			if (rotPuertaGarage > 90) {//Cambiar de estado
				estadoGarage1 = false;
				estadoGarage2 = true;
			}
		}
		if (estadoGarage2) {//Estado2 Mover puerta adentro del garage
			posPuertaGarage += 0.1f;
			if (posPuertaGarage > 9) {//Cambiar de estado
				estadoGarage2 = false;
				estadoGarage3 = true;
			}
		}
		if (estadoGarage3) {//Estado3 Mover el coche adentro del garage
			posCoche += 2.0f;
			if (posCoche > 800) {//Cambiar de estado
				estadoGarage3 = false;
				estadoGarage4 = true;
			}
		}
		if (estadoGarage4) {//Estado4 Sacar la puerta del garage
			posPuertaGarage -= 0.1f;
			if (posPuertaGarage < 0) {//Cambiar de estado
				estadoGarage4 = false;
				estadoGarage5 = true;
			}
		}
		if (estadoGarage5) {//Estado5 Rotar puerta para cerrarla
			rotPuertaGarage -= 0.3f;
			if (rotPuertaGarage < 0) {//Terminación de la animación
				estadoGarage5 = false;
				
			}
		}
		
		
	}
}

//La función realiza la lógica de la animación del movimiento del perro
void animacionPerro() {

	if (animPerro) { //Si se activa la animación
		if (estadoPerro1) {//Estado1 Mover al perro hacia adelante
			posPerro += 0.1f;
			if (posPerro > 20) {//Cambiar al siguiente estado
				estadoPerro1 = false;
				estadoPerro2 = true;
				
			}
		}
		if (estadoPerro2) {//Estado2 Rotar al perro para ver en sentido contrario y seguir moviéndose
			rotPerro = 180.0f;
			
			posPerro -= 0.1f;
			if (posPerro < 0) {//Cambio de estado
				estadoPerro2 = false;
				estadoPerro3 = true;

			}
		}

		if (estadoPerro3) {//Estado3 Colocar al perro en la posisción inicial
			rotPerro = 0.0f;
			posPerro = 0.0f;
			estadoPerro3 = false;
			estadoPerro1 = true;
			animPerro = false;

		}
	}
}

// La función se manda a llamar cada vez que se presiona una tecla mediante GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_P]) {
		animPuerta = true;
	}

	if (keys[GLFW_KEY_V]) {
		animVentilador = true;
	}

	
}

//La función recibe todas las entradas del mouse (Posición en X y Y)
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}