#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

#include "Camera.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
//Input interaction variables
bool keys[1024];
GLfloat lastX, lastY, xChange, yChange;
bool mouseFirstMoved;

const float toRadians = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

GLfloat deltatime = 0.0f;
GLfloat lastTime = 0.0f;


//GLuint VBO, VAO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		// Sides of the pyramid
		0, 4, 1, // Front-left face
		1, 4, 2, // Front-right face
		2, 4, 3, // Back-right face
		3, 4, 0, // Back-left face
		// Base of the pyramid
		0, 1, 2, // First triangle of the base
		2, 3, 0  // Second triangle of the base
	};

	GLfloat vertices[] = {
		// Base vertices of the first pyramid
		-1.0f, -1.0f, -1.0f, // Vertex 0: bottom left back
		1.0f, -1.0f, -1.0f,  // Vertex 1: bottom right back
		1.0f, -1.0f, 1.0f,   // Vertex 2: bottom right front
		-1.0f, -1.0f, 1.0f,  // Vertex 3: bottom left front
		// Apex vertex
		0.0f, 1.0f, 0.0f     // Vertex 4: top
	};

	unsigned int indices2[] = {
		// Sides of the pyramid
		0, 4, 1, // Front-left face
		1, 4, 2, // Front-right face
		2, 4, 3, // Back-right face
		3, 4, 0, // Back-left face
		// Base of the pyramid
		0, 1, 2, // First triangle of the base
		2, 3, 0  // Second triangle of the base
	};

	GLfloat vertices2[] = {
		// Base vertices of the second pyramid
		-1.0f, 1.0f, -1.0f, // Vertex 0: top left back
		1.0f, 1.0f, -1.0f,  // Vertex 1: top right back
		1.0f, 1.0f, 1.0f,   // Vertex 2: top right front
		-1.0f, 1.0f, 1.0f,  // Vertex 3: top left front
		// Apex vertex
		0.0f, -1.0f, 0.0f     // Vertex 4: bottom
	};



	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 16, 18);
	meshList.push_back(obj1);


	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices2, indices2, 16, 18);
	meshList.push_back(obj2);
}
void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

static void handleMouse(GLFWwindow* window, double xPos, double yPos) {
	if (mouseFirstMoved) {
		lastX = xPos;
		lastY = yPos;
		mouseFirstMoved = false;
	}
	xChange = xPos - lastX;
	yChange = lastY - yPos;

	lastX = xPos;
	lastY = yPos;
}
void InitialiseKeys() {
	for (size_t i = 0; i < 1024; i++) {

		keys[i] = 0;
	}
}

static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {

	//theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key <= 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

GLfloat getXChange() {
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}
GLfloat getYChange() {
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


int main()
{
	InitialiseKeys();

	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	
	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Callbacks
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow,NULL);

	CreateObjects();
	CreateShaders();
	
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Managin delta time for slow down interaction movment
		GLfloat now = glfwGetTime();
		deltatime = now - lastTime;
		lastTime = now;
		// Get + Handle user input events
		glfwPollEvents();
		//Check for keys presses
		camera.keyControl(keys, deltatime);
		//Mouse control
		camera.mouseControl(getXChange(), getYChange());

		if (direction)
		{
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (std::abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.0001f;
		}
		else {
			curSize -= 0.0001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();*/

		glUseProgram(0);


		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}
