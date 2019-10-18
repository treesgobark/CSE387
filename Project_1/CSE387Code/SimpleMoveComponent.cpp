#include "SimpleMoveComponent.h"



SimpleMoveComponent::SimpleMoveComponent()
{


}




void SimpleMoveComponent::update(float deltaTime)
{
	vec3 velocity = speed * moveDirection;

	this->owningGameObject->localTransform = 
		this->owningGameObject->localTransform 
		* glm::translate(velocity * deltaTime)
		* glm::rotate(glm::radians(rotationDegrees) * deltaTime, UNIT_Y_V3);

	roarTimer += deltaTime;
}


void SimpleMoveComponent::processInput()
{
	GLFWwindow* win = glfwGetCurrentContext();
	
	moveDirection = ZERO_V3;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) moveDirection += UNIT_X_V3;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) moveDirection += NEG_UNIT_X_V3;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) moveDirection += NEG_UNIT_Z_V3;
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) moveDirection += UNIT_Z_V3;
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) moveDirection += UNIT_Y_V3;
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) moveDirection += NEG_UNIT_Y_V3;
	if (glm::length(moveDirection) != 0) moveDirection = moveDirection / glm::length(moveDirection);

	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		speed = 20.0;
	}
	else {
		speed = 3.0;
	}

	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) rotationDegrees = 90.0f;
	else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationDegrees = -90.0f;
	else rotationDegrees = 0.0f;

	int leftButtonState = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LAST);

	if (leftButtonState == GLFW_PRESS && roarTimer > 2.0f) {
		cout << "Roar!!" << endl;
		roarTimer = 2.0f;
	}

	//double xpos, ypos;

	//glfwGetCursorPos(win, &xpos, &ypos);

	//static double xPrevPos = xpos, yPrevPos = ypos;

	//float rotationInc = (xpos - xPrevPos);

	//rotationDegrees = -rotationInc;

	//cout << xpos << " " << ypos << endl;

	//xPrevPos = xpos;
	//yPrevPos = ypos;
}

int SimpleMoveComponent::getMotionKeyCountArrow() {
	GLFWwindow* win = glfwGetCurrentContext();
	int count = 0;
	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) count++;
	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) count++;
	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) count++;
	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) count++;
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) count++;
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) count++;
	//if (glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) count++;
	return count;
}