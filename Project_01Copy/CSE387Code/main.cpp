#include "main.h"

GLuint shaderProgram; // Variable to hold the integer identifier for the shader program

ModelMesh dino1("Assets/Dinosaur/Trex.obj");

GLFWwindow* mWindow; // The rendering window

const GLint FRAMES_PER_SECOND = 60; // Desired maximum number of frames per second
const GLdouble FRAME_INTERVAL = 1.0 / FRAMES_PER_SECOND; // Interval in milliseconds between frames
float deltaTime = 0.0;

std::vector<GameObject*> gameObjects;


void window_close_callback(GLFWwindow* window) {

	// Destroy the window
	glfwDestroyWindow(window);

} // end window_close_callback

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);

	// Set the modeling transformation
	glm::mat4 projectionTransformation = glm::perspective(PI / 4.0f, ((float)width) / height, 0.5f, 1000.0f);
	projectionTransformation = projectionTransformation * glm::translate(vec3(0, 0, -50));
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

} // end framebuffer_size_callback

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	glm::mat4 projectionTransformation;
	glGetUniformfv(shaderProgram, 0, glm::value_ptr(projectionTransformation));
	if (action == GLFW_PRESS) {

		switch (key) {

		case GLFW_KEY_ESCAPE:

			glfwSetWindowShouldClose(window, GL_TRUE);

			break;

		case GLFW_KEY_UP:

			projectionTransformation = projectionTransformation * glm::rotate(glm::radians(10.0f), vec3(0, 1, 0));
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

			break;

		case GLFW_KEY_DOWN:

			projectionTransformation = projectionTransformation * glm::rotate(glm::radians(-10.0f), vec3(0, 1, 0));
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(projectionTransformation));

			break;

		default:

			fprintf(stdout, "key pressed\n");
		}

	}

} // end key_callback

void error_callback(int error, const char* description) {

	fprintf(stderr, "Error: %s\n", description);

} // end error_callback

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


void initialize()
{
	// Display OpenGL context information (OpenGL and GLSL versions) on the
	// command console.
	displayOpenGlInfo();

	// Register OpenGL error callback to print error messages to the console
	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	// Initialize OpenGL 
	glEnable(GL_DEPTH_TEST); // Turn on depth testing

	glClearColor(static_cast<GLclampf>(0.2), static_cast<GLclampf>(0.5), static_cast<GLclampf>(0.8), static_cast<GLclampf>(1.0));

	// Build shader program
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/vertexShader.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "Shaders/fragmentShader.fs.glsl" },
		{ GL_NONE, NULL } // signals that there are no more shaders 
	};

	shaderProgram = BuildShaderProgram(shaders);

	glUseProgram(shaderProgram);

	SharedProjectionAndViewing::setUniformBlockForShader(shaderProgram);
	SharedMaterialProperties::setUniformBlockForShader(shaderProgram);
	SharedGeneralLighting::setUniformBlockForShader(shaderProgram);

	SharedGeneralLighting::setDiffuseColor(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	SharedGeneralLighting::setPositionOrDirection(GL_LIGHT_ZERO, vec4(1.0f, 1.0f, 1.0f, 0.0f));
	SharedGeneralLighting::setEnabled(GL_LIGHT_ZERO, true);

	dino1.initialize(shaderProgram);

	GameObject* obj = new GameObject(nullptr);

	gameObjects.push_back(obj);

	for (auto gameObject : gameObjects) {
		gameObject->initialize();
	}

} // end intialize

/**
* Acts as the display function for the window. It is called every
* the window needs to be repainted. Registered with
* GLUT by calling glutDisplayFunc.
*/
static void render_scene_callback()
{
	// clear the both the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	dino1.draw();

	// flush all drawing commands and swap the front and back buffers
	glfwSwapBuffers(mWindow);

} // end RenderSceneCB

void update()
{
	// Compute delta time
	static double lastRenderTime = glfwGetTime();
	double currentTime = glfwGetTime();
	deltaTime = static_cast<float>(currentTime - lastRenderTime);

	if (deltaTime >= FRAME_INTERVAL) {

		if (deltaTime > 0.05f) {
			deltaTime = 0.05f;
		}

		// Set the viewing transformation
		mat4 viewingTransformation = glm::lookAt(vec3(0.0f, 5.0f, 20.0f), vec3(0.0f, 5.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(viewingTransformation));

		dino1.modelMatrix = glm::rotate(glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));// glm::mat4(1.0f);

		lastRenderTime = currentTime;

	}

}


void GLAPIENTRY 
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}


int main(int argc, char** argv)
{
	glfwInit();

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Use the core OpenGL profile	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int mScreenWidth = 1024;
	int mScreenHeight = 768;

	// Create rendering window and the OpenGL context.
	mWindow = glfwCreateWindow(mScreenWidth, mScreenHeight, "My First Window", NULL, NULL);

	if (!mWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mWindow);

	// Set the swap interval for the OpenGL context i.e. the number of screen updates 
	// to wait between before swapping the buffer and returning.
	glfwSwapInterval(1);

	// Prevent possible initialization error when using the core context
	glewExperimental = GL_TRUE;

	GLenum res = glewInit(); // Must be done after glut is initialized!

	if (res != GLEW_OK) {

		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false;
	}

	// On some platforms, GLEW will emit a benign error code. So clear it.
	glGetError();
	
	// Set function to be called whenever the frame buffer is resized.
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetWindowCloseCallback(mWindow, window_close_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetErrorCallback(error_callback);

	// ************ OpenGL initialization ***********************
	// During init, enable debug output

	initialize();
	framebuffer_size_callback(mWindow, 1024, 768);


	while (!glfwWindowShouldClose(mWindow)) {

		// Update the scene
		update();

		// Render Meshes
		render_scene_callback();

		// Processes events that are already in the event queue by 
		// calling registered window and input callback functions 	
		glfwPollEvents();
	}

	glDeleteShader(shaderProgram);

	// Frees all remaining resources allocated by GLFW
	glfwTerminate();

	//vec3 a = 2.0f * vec3(0.707, -0.707, 0.0);
	//vec3 n(0.0, 1.0, 0.0);
	//vec3 b(-1.0, 0.0, 0.0);


	//vec3 projbA = (glm::dot(a, b) / glm::dot(b, b)) * b;

	//vec3 r = projbA - (a - projbA);

	//cout << r << endl;

	
	return 0;
}
