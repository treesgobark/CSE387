#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MathLibsConstsFuncs.h"

// Variable to hold the integer identifier for the shader program
GLuint shaderProgram;

// Variable to hold the integer identifier for the vertex array object
GLuint vertexArrayObject;

GLFWwindow* mWindow;

void window_close_callback(GLFWwindow* window) {
	// Destroy the window
	glfwDestroyWindow(window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (action == GLFW_PRESS)
		cout << "Key pressed" << endl;
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
	// Display OpenGL context information on the command console.
	displayOpenGlInfo();

	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);

	// Set the clear color
	glClearColor(0.3f, 0.0, 0.0, 1.0);

	// Build shader program
	//TODO

	//Generate vertex array object and bind it for the first time
	//TODO

} // end initialize

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void render_scene_callback()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind vertex array object
	//TODO

	// Use the shader program
	//TODO

	// Fetch input data for pipeline	
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(mWindow);

} // end RenderSceneCB

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

	// Load vertex and texture data
	while (!glfwWindowShouldClose(mWindow)) {
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: replace this

		// Render Meshes
		// TODO
		// Swap the front and back buffers
		glfwSwapBuffers(mWindow); // TODO: repalce this

		render_scene_callback();

		// Processes events that are already in the event queue by 
		// calling registered window and input callback functions 
		glfwPollEvents();
	}

	// Frees other allocated resources
	glfwTerminate();

	return 0;
}

