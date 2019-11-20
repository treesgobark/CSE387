#include "SimpleMoveComponent.h"



SimpleMoveComponent::SimpleMoveComponent()
{


}


void SimpleMoveComponent::update(float deltaTime)
{
	//mat4 localTrans = this->owningGameObject->sceneNode.getTransformation(WORLD);

	//localTrans = localTrans *
	//	glm::translate(moveVector * deltaTime * speed) *
	//	glm::rotate(glm::radians(rotationDegrees), UNIT_Y_V3);
	//
	//this->owningGameObject->sceneNode.setTransform(localTrans, WORLD);

	//roarTimer += deltaTime;
}


void SimpleMoveComponent::processInput()
{
	// Polling vs callbacks
	GLFWwindow* win = glfwGetCurrentContext();

	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {

		this->moveVector = owningGameObject->sceneNode.getRightDirection(LOCAL);

	}
	else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {

		this->moveVector = -owningGameObject->sceneNode.getRightDirection(LOCAL);

	}
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {

		this->moveVector = -owningGameObject->sceneNode.getFowardDirection(LOCAL);

	}
	else {

		this->moveVector = owningGameObject->sceneNode.getFowardDirection(LOCAL);
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

void SimpleMoveComponent::CollisionEnter(const RigidBodyComponent* collisionData)
{

	cout << "BANG!!" << endl;



}