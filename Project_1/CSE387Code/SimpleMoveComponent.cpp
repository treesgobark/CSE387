#include "SimpleMoveComponent.h"



SimpleMoveComponent::SimpleMoveComponent()
{


}




void SimpleMoveComponent::update(float deltaTime)
{
	vec3 velocity = speed * moveDirection;

	this->owningGameObject->localTransform = 
		this->owningGameObject->localTransform 
		* glm::translate(velocity * deltaTime);
}


void SimpleMoveComponent::processInput()
{
	GLFWwindow* win = glfwGetCurrentContext();
	
	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		moveDirection = NEG_UNIT_X_V3;
	} else if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		moveDirection = UNIT_X_V3;
	} else if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		moveDirection = UNIT_Z_V3;
	} else if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		moveDirection = NEG_UNIT_Z_V3;
	} else {
		moveDirection = ZERO_V3;
	}

	int leftButtonState = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LAST);

	if (leftButtonState == GLFW_PRESS) {
		cout << "Roar!!" << endl;
	}
}