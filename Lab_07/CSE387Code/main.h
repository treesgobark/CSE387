#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "MathLibsConstsFuncs.h"

#include "BuildShaderProgram.h"
#include "SharedProjectionAndViewing.h"
#include "SharedMaterialProperties.h"
#include "SharedGeneralLighting.h"
#include "ModelMeshComponent.h"

/**
* Declarations for functions that act as "call backs" for various
* events. GLFW commands are used to register most of these functions
* as event handlers.
*/

/**
 * @fn	static void render_scene_callback();
 *
 * @brief	Issues OpenGL rendering commands and swaps front and back buffers. It is called every
 * 			the window needs to be repainted. Called explicitly in the rendering loop.
 */
static void render_scene_callback();

/**
 * @fn	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
 *
 * @brief	Called whenever the framebuffer is resized. Registered with GLFW by calling
 * 			glfwSetFramebufferSizeCallback. Resets the graphics viewport limits for full window
 * 			rendering each time the framebuffer is resized.
 *
 * @param [in,out]	window	- The OpenGL rendering context.
 * @param 		  	width 	of the window in pixels.
 * @param 		  	height	of the window in pixels.
 */
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * @fn	void window_close_callback(GLFWwindow* window);
 *
 * @brief	Callback, called when the window closed. "Destroys" the closed window.
 *
 * @param [in]	window	If non-null, the window that was closed.
 */
void window_close_callback(GLFWwindow* window);

/**
 * @fn	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
 *
 * @brief	Calls a method of the the OpenGLApplicationBase class. Registered with GLUT by
 * 			calling glutKeyboardFunc.
 *
 * @param [in,out]	window  	- OpenGL rendering context.
 * @param 		  	key			- Keyboard key that was pressed or released.
 * @param 		  	scancode	- System-specific scancode of the key.
 * @param 		  	action  	- GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 * @param 		  	mods		- Bit field describing which modifier keys were held down.
 */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/**
 * @fn	static void error_callback(int error, const char* description);
 *
 * @brief	Called when GLFW detects an error. Registered with GLFW by calling
 * 			glfwSetErrorCallback.
 *
 * @param	error	   	- Error code.
 * @param	description	- UTF-8 encoded string describing the error.
 */
static void error_callback(int error, const char* description);

/**
 * @fn	static void displayOpenGlInfo(void);
 *
 * @brief	Function: Displays OpengGL and GLSL version information.
 */
static void displayOpenGlInfo(void);

/**
 * @fn	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
 *
 * @brief	Callback, called when the message
 *
 * @param	source   	Source for the.
 * @param	type	 	The type.
 * @param	id		 	The identifier.
 * @param	severity 	The severity.
 * @param	length   	The length.
 * @param	message  	The message.
 * @param	userParam	The user parameter.
 *
 * @returns	A GLAPIENTRY.
 */
void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);