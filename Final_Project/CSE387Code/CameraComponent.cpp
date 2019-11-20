#include "CameraComponent.h"

std::vector<CameraComponent*> CameraComponent::activeCameras;
CameraComponent::CameraComponent(int updateOrder)
	:Component(updateOrder)
{
	this->componentName = "camera";

}

void CameraComponent::initialize()
{
	activeCameras.push_back(this);
	std::sort(activeCameras.begin(), activeCameras.end(), CompareDepth);
}

/**
* Called before the scene is rendered from the perspective of this camera.
* Sets the viewport, projection matrix, and viewing transformation based upon this Camera so
* that the scene can be rendered in the specified viewport without distortion from the
* viewpoint of the Camera. Uses methods of the SharedProjectionAndViewing class to accomplish
* this task.
*/
void CameraComponent::setViewingTransformation()
{
	int width, height;
	// Get the current width and height of the rendering window
	width = this->owningGameObject->getOwningGame()->getWindowWidth();
	height = this->owningGameObject->getOwningGame()->getWindowHeight();

	glViewport(static_cast<GLint>(xLowerLeft * width), 
		static_cast<GLint>(yLowerLeft * height), 
		static_cast<GLint>(viewPortWidth * width), 
		static_cast<GLint>(viewPortHeight * height));

	SharedProjectionAndViewing::setViewMatrix(glm::inverse(this->owningGameObject->sceneNode.getTransformation(WORLD)));

	//	GLfloat aspect = static_cast<float>(width) / height;
	GLfloat aspect = static_cast<float>(viewPortWidth * width) / (height * viewPortHeight);

	mat4 projMat = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 1000.0f);

	SharedProjectionAndViewing::setProjectionMatrix(projMat);

}

/**
* Sets the rendering area for the camera. In normalized coordinate the width and height
* of the viewport are 1.0.
	*
	* @param xLowerLeft normalized x coordinate of the lower left hand corner or the viewport
	* @param yLowerLeft normalized y coordinate of the lower left hand corner or the viewport
	* @param viewPortWidth normalized width of the view port in pixels
	* @param viewPortHeight normalized height of the view port in pixels
	*/
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
	std::sort(activeCameras.begin(), activeCameras.end(), CompareDepth);
}
