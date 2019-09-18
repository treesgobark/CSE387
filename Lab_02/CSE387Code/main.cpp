#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "MathLibsConstsFuncs.h"
#include "BuildShaderProgram.h"

// Variable to hold the integer identifier for the shader program
GLuint shaderProgram;

// Variable to hold the integer identifier for the vertex array object
GLuint vertexArrayObject;

GLuint mode = 1;

GLFWwindow* mWindow;

// This will identify our vertex buffer
GLuint vertexbuffers[3];

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

	string textureFileName = "fileName";

	//Generate vertex array object and bind it for the first time
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data1[] = {
	   -0.7f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.0f,
	   -0.3f,  0.0f,  0.0f,
	    0.3f,  0.0f,  0.0f,
	    0.5f,  0.5f,  0.0f,
	    0.7f,  0.0f,  0.0f,
	   -0.2f,  0.0f,  0.0f,
	    0.0f, -0.5f,  0.0f,
	    0.2f,  0.0f,  0.0f,
	};

	static const GLfloat g_vertex_buffer_data2[] = {
		-0.75f,  0.5f,  0.0f,
		 -0.5f,  0.5f,  0.0f,
		-0.75f,  0.5f,  0.0f,
		  0.5f,  0.5f,  0.0f,
	 	 0.75f,  0.5f,  0.0f,
		-0.75f, -0.5f,  0.0f,
		 -0.5f, -0.5f,  0.0f,
		-0.75f, -0.5f,  0.0f,
		  0.5f, -0.5f,  0.0f,
		 0.75f, -0.5f,  0.0f,
	};

	static const GLfloat g_vertex_buffer_data3[] = {
		-0.75f,   0.0f,  0.0f,
		  0.0f, -0.75f,  0.0f,
		 0.75f,   0.0f,  0.0f,
		  0.0f,  0.75f,  0.0f,
	};

	// Generate 3 buffers, put the resulting identifier in vertexbuffer
	glGenBuffers(3, vertexbuffers);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[0]);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data1), g_vertex_buffer_data1, GL_STATIC_DRAW);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[1]);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[2]);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data3), g_vertex_buffer_data3, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

} // end initialize

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted.
*/
static void render_scene_callback()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[mode-1]);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Bind vertex array object
	// glBindVertexArray(vertexArrayObject);

	// Use the shader program
	glUseProgram(shaderProgram);

	// Fetch input data for pipeline	
	switch (mode) {
	case 1: glDrawArrays(GL_TRIANGLES, 0, 9);
		break;
	case 2: glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
		break;
	case 3: glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
		break;
	}
	glDisableVertexAttribArray(0);

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
		render_scene_callback();

		// Processes events that are already in the event queue by 
		// calling registered window and input callback functions 
		glfwPollEvents();
	}

	glDeleteShader(shaderProgram);
	glDeleteVertexArrays(1, &vertexArrayObject);

	// Frees other allocated resources
	glfwTerminate();

	return 0;
}

