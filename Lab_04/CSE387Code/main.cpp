#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "FigureOne.h"
#include "FigureTwo.h"
#include "MathLibsConstsFuncs.h"
#include "BuildShaderProgram.h"

// Variable to hold the integer identifier for the shader program
GLuint shaderProgram;

// Variable to hold the integer identifier for the vertex array object
GLuint vertexArrayObjects[3];

GLuint mode = 1;

GLFWwindow* mWindow;

GLuint texBufferID;
GLuint texCoordID, texID;

// This will identify our vertex buffer
GLuint vertexbuffers[3];

float rotationX = 0.3f;
float rotationY = 0.5f;
float rotationZ = 0.7f;

FigureOne figureOne;
FigureTwo figureTwo;
FigureTwo cube1;
FigureTwo cube2;

void window_close_callback(GLFWwindow* window) {
	// Destroy the window
	glfwDestroyWindow(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	mat4 projectionTransform = glm::perspective(glm::radians(45.0f), (float)width / (float)height, .5f , 100.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransform));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_1:
			mode = 1;
			break;
		case GLFW_KEY_2:
			mode = 2;
			break;
		case GLFW_KEY_3:
			mode = 3;
			break;
		case GLFW_KEY_X:
			mode = 1;
			break;
		case GLFW_KEY_Y:
			mode = 2;
			break;
		case GLFW_KEY_Z:
			mode = 3;
			break;
		}
	}
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

/**
Function: Displays OpengGL and GLSL version information.
*/
void displayOpenGlInfo(void)
{
	// Display the company responsible for this GL implementation
	fprintf(stdout, "OpenGL INFO:\n\tOpenGL Implementor: %s\n",
		glGetString(GL_VENDOR));
	// Display the renderer/graphics card
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	// Display the OpenGL version that is implemented
	fprintf(stdout, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	// Display the GLSL version
	fprintf(stdout, "\tGLSL Version: %s\n\n",
		glGetString(GL_SHADING_LANGUAGE_VERSION));

} // end displayOpenGlInfo

/**
* Initialize OpenGL. Builds a shader program and creates a vertex
* array object.
*/
void initialize()
{
	cout << "big kek" << endl;
	// Display OpenGL context information on the command console.
	displayOpenGlInfo();

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);

	// Set the clear color
	glClearColor(0.3f, 0.0, 0.0, 1.0);
	
	// Build shader program
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};
	
	shaderProgram = BuildShaderProgram(shaders);

	figureOne.initialize(shaderProgram);
	figureTwo.initialize(shaderProgram);
	cube1.initialize(shaderProgram);
	cube2.initialize(shaderProgram);

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(mat4(1.0f)));
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(mat4(1.0f)));

	Texture brickTexture;
	brickTexture.load("Textures/BRICK.bmp");
	figureOne.setTexture(brickTexture);
	figureTwo.setTexture(brickTexture);

} // end initialize

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void render_scene_callback()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 viewTransform = glm::lookAt(vec3(0.0f, 0.0f, -3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewTransform));
	//cout << viewTransform << endl;

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);

	// Fetch input data for pipeline	
	/*switch (mode) {
	case 1:
		figureOne.draw();
		break;
	case 2:
		figureTwo.draw();
		break;
	case 3:
		cube1.draw();
		cube2.draw();
		break;
	}*/
	figureTwo.draw();
	glDisableVertexAttribArray(0);

	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(mWindow);

} // end RenderSceneCB

void update() {
	/*switch (mode) {
	case 1:
		figureOne.modelMatrix = figureOne.modelMatrix * glm::rotate(glm::radians(1.0f), vec3(0, 1, 0));
		break;
	case 2:
		figureTwo.modelMatrix = figureTwo.modelMatrix *
			glm::rotate(glm::radians(sin(rotationX/2)), vec3(1, 0, 0)) *
			glm::rotate(glm::radians(sin(rotationY/2)), vec3(0, 1, 0)) *
			glm::rotate(glm::radians(sin(rotationZ/2)), vec3(0, 0, 1));
		//figureOne.modelMatrix = figureOne.modelMatrix *
		//	glm::rotate(glm::radians(1.0f), vec3(0, 1, 0));
		break;
	case 3: 
		cube1.modelMatrix = cube1.modelMatrix *
			glm::translate(mat4(), vec3(0.01f, 0.0f, 0.0f)) *
			glm::rotate(glm::radians(1.0f), vec3(0, 1, 0));
		cube2.modelMatrix = cube2.modelMatrix *
			glm::translate(mat4(), vec3(-0.01f, 0.0f, 0.0f)) *
			glm::rotate(glm::radians(1.0f), vec3(0, -1, 0));
		//figureOne.modelMatrix = figureOne.modelMatrix *
		//	glm::rotate(glm::radians(1.0f), vec3(1, 0, 0));
		break;
	}*/
	switch (mode) {
	case 1:
		figureTwo.modelMatrix = figureTwo.modelMatrix * glm::rotate(glm::radians(1.0f), vec3(1, 0, 0));
		break;
	case 2:
		figureTwo.modelMatrix = figureTwo.modelMatrix * glm::rotate(glm::radians(1.0f), vec3(0, 1, 0));
		break;
	case 3:
		figureTwo.modelMatrix = figureTwo.modelMatrix * glm::rotate(glm::radians(1.0f), vec3(0, 0, 1));
		break;
	}
}

int main(int argc, char** argv)
{
	glfwInit();
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Use the core OpenGL profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int mScreenWidth = 1024;
	int mScreenHeight = 768;

	// Create rendering window and the OpenGL context.
	mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "My First Window - Logan Petruziello", NULL, NULL);
	if (!mWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mWindow);

	// Set the swap interval for the OpenGL context i.e. the number of screen
	// updates to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	// Prevent possible initialization error when using the core context
	glewExperimental = GL_TRUE; GLenum res = glewInit(); // Must be done after glut is initialized!
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));return false;
	}
	
	// On some platforms, GLEW will emit a benign error code. So clear it.
	glGetError();

	// Ain't No Callaback Girl
	glfwSetWindowCloseCallback(mWindow, window_close_callback);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetErrorCallback(error_callback);

	initialize();

	mat4 projectionTransform = glm::perspective(glm::radians(45.0f), (float)mScreenWidth / (float)mScreenHeight, .5f, 100.0f);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransform));

	// Load vertex and texture data
	while (!glfwWindowShouldClose(mWindow)) {
		update();

		render_scene_callback();

		// Processes events that are already in the event queue by 
		// calling registered window and input callback functions 
		glfwPollEvents();
	}

	glDeleteShader(shaderProgram);
	glDeleteVertexArrays(3, vertexArrayObjects);

	// Frees other allocated resources
	glfwTerminate();

	return 0;
}

