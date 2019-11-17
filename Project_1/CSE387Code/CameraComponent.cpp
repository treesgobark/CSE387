#include "CameraComponent.h"

CameraComponent::CameraComponent(int updateOrder)
{
	initialize();
}

void CameraComponent::initialize()
{
	CameraComponent::activeCameras.push_back(this);
	std::sort(CameraComponent::activeCameras.begin(), CameraComponent::activeCameras.end(), CompareDepth);
}

void CameraComponent::setViewingTransformation()
{
	mat4 projMatrix = glm::inverse(getOwningGameObject()->sceneNode.getTransformation(WORLD));

	mat4 viewMatrix = glm::lookAt(getOwningGameObject()->sceneNode.getPosition(WORLD),
								  getOwningGameObject()->sceneNode.getFowardDirection(WORLD),
								  getOwningGameObject()->sceneNode.getUpDirection(WORLD));

	SharedProjectionAndViewing::setProjectionMatrix(projMatrix);
	SharedProjectionAndViewing::setViewMatrix(viewMatrix);

	glViewport(0, 0, getOwningGameObject()->getOwningGame()->getWindowWidth(), getOwningGameObject()->getOwningGame()->getWindowHeight());
}

void CameraComponent::setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight)
{
	this->xLowerLeft = xLowerLeft;
	this->yLowerLeft = yLowerLeft;
	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = viewPortHeight;
}

void CameraComponent::setDepth(int depth)
{
	this->depth = depth;
}

std::vector<CameraComponent*> CameraComponent::activeCameras;