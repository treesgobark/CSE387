#include "SimpleMoveComponent.h"



SimpleMoveComponent::SimpleMoveComponent()
{


}


void SimpleMoveComponent::update(float deltaTime)
{
	mat4 localTrans = this->owningGameObject->sceneNode.getTransformation(WORLD);

	localTrans = localTrans *
		glm::translate(this->moveVector * deltaTime * speed) *
		glm::rotate(glm::radians(rotationDegrees), UNIT_Y_V3);
	
	this->owningGameObject->sceneNode.setTransform(localTrans, WORLD);

	roarTimer += deltaTime;
}


void SimpleMoveComponent::processInput()
{
	// Polling vs callbacks
	GLFWwindow* win = glfwGetCurrentContext();

	if (glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS) {

		this->moveVector = UNIT_X_V3;

	}
	else if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS) {

		this->moveVector = UNIT_Z_V3;

	}
	else {

		this->moveVector = vec3(0.0f, 0.0f, 0.0f);
	}

	int leftButtonState = glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT);


	if (leftButtonState == GLFW_PRESS && roarTimer > 1.0f)
	{
		cout << "Roar!" << endl;
		roarTimer = 0;
	}

	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {

		this->speed += 0.1f;

	}
	else if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {

		this->speed -= 0.1f;

	}

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	static double xposPrev = xpos, yposPrev = ypos;

	double rotationInc = (xpos - xposPrev);

	rotationDegrees += static_cast<float>(rotationInc / 100.0);

	xposPrev = xpos;
	yposPrev = ypos;

	//cout << xpos << " " << ypos << " " << rotationInc << endl;
}