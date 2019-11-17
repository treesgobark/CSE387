#include "CameraComponent.h"

CameraComponent::CameraComponent(int updateOrder)
{
}

void CameraComponent::initialize()
{
	CameraComponent::activeCameras.push_back(this);
	std::sort(CameraComponent::activeCameras.begin(), CameraComponent::activeCameras.end(), CompareDepth);
}

void CameraComponent::setViewingTransformation()
{
}

void CameraComponent::setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight)
{
	this->xLowerLeft = xLowerLeft;
	this->yLowerLeft = yLowerLeft;
	this->viewPortWidth = viewPortWidth;
	this->viewPortHeight = this->viewPortHeight;
}

void CameraComponent::setDepth(int depth)
{
	this->depth = depth;
}

std::vector<CameraComponent*> CameraComponent::activeCameras;