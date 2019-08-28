#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MathLibsConstsFuncs.h"

void window_close_callback(GLFWwindow* window) {
	// Destroy the window
	glfwDestroyWindow(window);
}

int main(int argc, char** argv)
{
	glfwInit();
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Use the core OpenGL profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int mScreenWidth = 500;
	int mScreenHeight = 300;

	// Create rendering window and the OpenGL context.
	GLFWwindow* mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "My First Window", NULL, NULL);
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

	glfwSetWindowCloseCallback(mWindow, window_close_callback);

	// Load vertex and texture data
	while (!glfwWindowShouldClose(mWindow)) {
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Meshes
		// TODO
		// Swap the front and back buffers
		glfwSwapBuffers(mWindow);

		// Processes events that are already in the event queue by 
		// calling registered window and input callback functions 
		glfwPollEvents();
	}

	// Frees other allocated resources
	glfwTerminate();

	return 0;
}
