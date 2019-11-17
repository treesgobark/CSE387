#pragma once
#include "Component.h"
#include <vector>
#include <algorithm> 
#include <iostream> 

#include "SharedProjectionAndViewing.h"

class CameraComponent : public Component
{
public:

	/**
	 * @fn	CameraComponent::CameraComponent(int updateOrder = 100);
	 *
	 * @brief	Constructor. By default, Cameras render to the entire window with a depth of zero.
	 * 			Constructed cameras are added to the static activeCameras vector.
	 *
	 * @param	updateOrder	(Optional) The update order.
	 */
	CameraComponent(int updateOrder = 100);

	/**
	 * @fn	void CameraComponent::initialize() override;
	 *
	 * @brief	Initializes the camera by adding it to the activeCameras vector and then sorts the vector
	 * 			base on camera depth.
	 */
	void initialize() override;

	/**
	 * @fn	void CameraComponent::setViewingTransformation();
	 *
	 * @brief	Called before the scene is rendered from the perspective of this camera. Sets the
	 * 			viewport (using glViewport), projection matrix, and viewing transformation based upon
	 * 			this Camera so that the scene can be rendered in the specified viewport without
	 * 			distortion from the viewpoint of the Camera. Uses methods of the
	 * 			SharedProjectionAndViewing class to accomplish this task.
	 */
	void setViewingTransformation();

	/**
	 * @fn	void CameraComponent::setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight);
	 *
	 * @brief	Sets the rendering area for the camera. In normalized coordinate the width and height
	 * 			of the viewport are 1.0.
	 *
	 * @param	xLowerLeft	  	normalized x coordinate of the lower left hand corner or the viewport.
	 * @param	yLowerLeft	  	normalized y coordinate of the lower left hand corner or the viewport.
	 * @param	viewPortWidth 	normalized width of the view port in pixels.
	 * @param	viewPortHeight	normalized height of the view port in pixels.
	 */
	void setViewPort(GLfloat xLowerLeft, GLfloat yLowerLeft, GLfloat viewPortWidth, GLfloat viewPortHeight);

	/**
	 * @fn	void CameraComponent::setDepth(int depth);
	 *
	 * @brief	Set the depth for the Camera to a non-negative value. View of the scene with the
	 * 			higher depth value is rendered on top of camera with lower depth value. Calls to this
	 * 			method should cause the activeCameras vector to be re-sorted by depth from smallest to
	 * 			largest each time this method is called.
	 *
	 * @param	depth	non-negative depth value for the camera.
	 */
	void setDepth(int depth);

	bool isActive = true;

	/**
	 * @brief	Vector containing the Cameras that are enabled. The vector should be sorted based
	 * 			upon the depth values of the cameras. Figure out how to the use STL sort method to
	 * 			make this happen.
	 */
	static std::vector<CameraComponent*> activeCameras;

	/**
	 * @fn	static bool CameraComponent::CompareDepth(CameraComponent* rightCamera, CameraComponent* leftCamera)
	 *
	 * @brief	Compare depth
	 *
	 * @param [in,out]	rightCamera	If non-null, a CameraComponent to right of inequality.
	 * @param [in,out]	leftCamera 	If non-null, a CameraComponent to left of inequality.
	 *
	 * @returns	True if the rightCamera depth is lower then the leftCamera depth, false otherwise.
	 */
	static bool CompareDepth(CameraComponent* rightCamera, CameraComponent* leftCamera)
	{
		return (rightCamera->depth < leftCamera->depth);
	}

protected:

	// Normalized viewport settings
	GLfloat xLowerLeft = 0.0f;
	GLfloat yLowerLeft = 0.0f;
	GLfloat viewPortWidth = 1.0f;
	GLfloat viewPortHeight = 1.0f;

	// Render depth of the camera. Higher depth cameras render on top of lower depth cameras.
	int depth = 0;

};

