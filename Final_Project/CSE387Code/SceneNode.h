#pragma once

#include "MathLibsConstsFuncs.h"

class SceneGraphNode
{
public:

	/**
	 * @fn	glm::vec3 SceneNode::getPosition(Frame frame = LOCAL);
	 *
	 * @brief	Gets the position of the scene graph node relative to either the World or local coordinate
	 * 			frame.
	 *
	 * @param	frame	(Optional) frame for the position.
	 *
	 * @returns	position relative to the specified coordinate frame.
	 */
	glm::vec3 getPosition(Frame frame = LOCAL);

	/**
	 * @fn	glm::mat4 SceneNode::getRotation(Frame frame = LOCAL);
	 *
	 * @brief	Gets the orientation of the scene graph node relative to either the World or local coordinate
	 * 			frame. 
	 *
	 * @param	frame	(Optional) frame for the rotation.
	 *
	 * @returns	orientation relative to the specified coordinate frame.
	 */
	glm::mat4 getRotation(Frame frame = LOCAL);

	/**
	 * @fn	glm::mat4 SceneNode::getScale(Frame frame = LOCAL);
	 *
	 * @brief	Gets the scale of the scene graph node relative to either the World 
	 * 			or local local coordinate frame.
	 *
	 * @param	frame	(Optional) frame for the position.
	 *
	 * @returns	scale relative to the specified coordinate frame.
	 */
	glm::mat4 getScale(Frame frame = LOCAL);

	/**
	 * @fn	glm::mat4 SceneNode::getTransformation(Frame frame = LOCAL);
	 *
	 * @brief	Returns the transformation this scene graph node relative to
	 * 			either the
	 *
	 * @param	frame	(Optional) The frame.
	 *
	 * @returns	transformation of this node relative to the specified coordinate
	 * 			frame.
	 */
	glm::mat4 getTransformation(Frame frame = LOCAL);

	/**
	 * @fn	void SceneNode::setPosition(glm::vec3 position, Frame frame = LOCAL);
	 *
	 * @brief	Set the position of the scene graph node relative to either the World or local coordinate
	 * 			frame. Orientation is not changed.
	 *
	 * @param	position	vector xyz coordinates of the position.
	 * @param	frame   	(Optional) frame relative to which the position is to be set.
	 */
	void setPosition(glm::vec3 position, Frame frame = LOCAL);

	/**
	 * @fn	void SceneNode::setRotation(glm::mat4 rotation, Frame frame = LOCAL);
	 *
	 * @brief	Set the orientation of the scene graph node relative to either the World or local coordinate
	 * 			frame. Position is not changed.
	 *
	 * @param	rotation	4 x 4 matrix describing the orientation.
	 * @param	frame   	(Optional) frame relative to which the rotation is to be set.
	 */
	void setRotation(glm::mat4 rotation, Frame frame = LOCAL);

	/**
	 * @fn	void SceneNode::setScale(const vec3& scale, Frame frame = LOCAL);
	 *
	 * @brief	Sets a scale for the scene graph node relative to either the World or local coordinate
	 * 			frame.
	 *
	 * @param	scale	The scale.
	 * @param	frame	(Optional) The frame.
	 */
	void setScale(const vec3& scale, Frame frame = LOCAL);

	/**
	 * @fn	void SceneNode::setTransform(glm::mat4 transformation, Frame frame = LOCAL);
	 *
	 * @brief	Sets a transform of the scene graph node relative to either the World or 
	 * 			local coordinate frame.
	 *
	 * @param	transformation	The transformation.
	 * @param	frame		  	(Optional) The frame.
	 */
	void setTransform(glm::mat4 transformation, Frame frame = LOCAL);

	/**
	 * @fn	glm::vec3 SceneNode::getFowardDirection(Frame frame = LOCAL);
	 *
	 * @brief	Returns the forward direction of the scene graph node relative to either the Local or World
	 * 			coordinate frame.
	 *
	 * @param	frame	(Optional) frame for the forward direction.
	 *
	 * @returns	facing direction relative to the specified coordinate frame.
	 */
	glm::vec3 getFowardDirection(Frame frame = LOCAL);

	/**
	 * @fn	glm::vec3 SceneNode::getUpDirection(Frame frame = LOCAL);
	 *
	 * @brief	Returns the up direction of the scene graph node relative to either the Local or World
	 * 			coordinate frame
	 *
	 * @param	frame	(Optional) frame for the up direction.
	 *
	 * @returns	up direction relative to the specified coordinate frame.
	 */
	glm::vec3 getUpDirection(Frame frame = LOCAL);

	/**
	 * @fn	glm::vec3 SceneNode::getRightDirection(Frame frame = LOCAL);
	 *
	 * @brief	Gets right direction f the scene graph node relative to either the Local or World
	 * 			coordinate frame.
	 *
	 * @param	frame	(Optional) The frame.
	 *
	 * @returns	The right direction.
	 */
	glm::vec3 getRightDirection(Frame frame = LOCAL);

	/**
	 * @fn	glm::mat4 SceneNode::getParentWorldTransform();
	 *
	 * @brief	Returns the World transformation of the parent of this scene graph node
	 *
	 * @returns	World transformation of the parent of this node.
	 */
	glm::mat4 getParentWorldTransform();

	/**
	 * @fn	glm::mat4 SceneNode::getModelingTransformation();
	 *
	 * @brief	Gets modeling transformation for this sceneGraphNode including
	 * 			both the world transformation and any scale applied to this node.
	 *
	 * @returns	The modeling transformation.
	 */
	glm::mat4 getModelingTransformation();

	/**
	 * @fn	void SceneGraphNode::rotateTo(const glm::vec3& direction, Frame frame) void setApplyScaleToChildren(bool applyScale)
	 *
	 * @brief	Rotate the object to "face' in the direction described by the input parameter
	 *
	 * @param	direction	The direction.
	 * @param	frame	 	The frame.
	 */
	void rotateTo(const glm::vec3& direction, Frame frame);

	/**
	 * @fn	void SceneGraphNode::eulerRotate(const float& yawAngle, const float& pitchAngle, const float& rollAngle, Frame frame);
	 *
	 * @brief	Rotates the scene graph node using Euler angles relative to either the LOCAL or WORLD 
	 * 			coordinate frame.
	 *
	 * @param	yawAngle  	The yaw angle.
	 * @param	pitchAngle	The pitch angle.
	 * @param	rollAngle 	The roll angle.
	 * @param	frame	  	The frame.
	 */
	void eulerRotate(const float& yawAngle, const float& pitchAngle, const float& rollAngle, Frame frame);

	/**
	 * @fn	void SceneNode::setApplyScaleToChildren(bool temp)
	 *
	 * @brief	Sets apply scale to children which determined if the scale
	 * 			of this scene graph node will be applied to the children
	 *
	 * @param	temp	True to temporary.
	 */
	void setApplyScaleToChildren(bool applyScale) { applyScaleToChildren = applyScale; }

	/**
	 * @fn	void SceneNode::reparent(class GameObject* child);
	 *
	 * @brief	Adds a child to this GameObject. Removes the child from the children of the former
	 * 			parent. Local transformation is set so that the position and orientation relative to
	 * 			the World reference frame is unchanged. Set the parent of the child to "this"
	 * 			GameObject.
	 *
	 * @param [in]	child	- child that is being added.
	 */
	void reparent(class GameObject* child);

	/**
	 * @fn	void Game::addGameObject(class GameObject* gameObject);
	 *
	 * @brief	Adds a game object the game.
	 *
	 * @param [in]	gameObject	If non-null, the game object to be added.
	 */
	void addGameObjectToSceneGraph(class GameObject* gameObject);

	/**
	 * @fn	void Game::removeGameObject(class GameObject* gameObject);
	 *
	 * @brief	Removes the game object described by gameObject from
	 * 			the collection of game objects that are in the game.
	 *
	 * @param [in]	gameObject	If non-null, the game object being removed.
	 */
	void removeGameObjectFromSceneGraph(class GameObject* gameObject);

	/**
	 * @fn	std::vector<class GameObject*> SceneGraphNode::getChildren() const
	 *
	 * @brief	Gets the children of this SceneGraphNode
	 *
	 * @returns	Null if it fails, else the children.
	 */
	std::vector<class GameObject*> getChildren() const
	{
		return children;
	}

	/**
	 * @fn	SceneGraphNode* SceneGraphNode::getParent()
	 *
	 * @brief	Gets the parent of this SceneGraphNode
	 *
	 * @returns	Null if the node does not have a parent, else the parent.
	 */
	SceneGraphNode* getParent() { return this->parent; }

	/**
	 * @fn	void SceneGraphNode::setParent(SceneGraphNode* parent)
	 *
	 * @brief	Sets the parent of this SceneGraphNode.
	 *
	 * @param [in]	parent	the new parent of the SceneGraphNode
	 */
	void setParent(SceneGraphNode* parent)  { this->parent = parent; }

	/**
	 * @fn	void SceneGraphNode::updateSceneGraph(float deltaTime);
	 *
	 * @brief	Updates the scene graph under the assumuption that the
	 * 			SceneGraphNode through which the method was called is the 
	 * 			root of the scene graph. Could possibly be used to update
	 * 			just a sub-branch of the scene graph.
	 *
	 * @param	deltaTime	The time in seconds since the last update
	 */
	void updateSceneGraph(float deltaTime);

	/**
	 * @fn	static void SceneGraphNode::designateDeadGameObject( GameObject * gameObject )
	 *
	 * @brief	Used to designate dead game object that should be removed
	 * 			from the game after the next update cycle has completed.
	 *
	 * @param [in,out]	gameObject	If non-null, the game object.
	 */
	static void designateDeadGameObject( GameObject * gameObject )
	{		
		// Add the this dead game object to static list of game objects
		// to be deleted on the next update cycle.
		SceneGraphNode::deadGameObjects.emplace_back(gameObject);
	}

	protected:
	
		/** @brief	Track if  gameObjects are being updated */
	static bool updatingGameObjects;

	///** @brief	All the GameObjects attached to this scene graph node */
	std::vector<class GameObject*> children;

	///** @brief	Any pending GameObjects that need to be added to the 
	// scene graph on the next update cycle. */
	static std::vector<class GameObject*> pendingChildren;

	/** @brief	The dead game objects that need to be removed from 
	the scene graph on the next update cycle */
	static std::vector<class GameObject*> deadGameObjects;

	/** @brief	True to apply scale to children. false otherwise */
	bool applyScaleToChildren = false;

	/**
	 * @brief	Local transform that expresses the position, orientation, and scale of this game
	 * 			object relative to its parent.
	 */
	mat4 localTransform = mat4(1.0f);

	/** @brief	4 x 4 matrix holding the scale for the game object */
	mat4 scale = mat4(1.0f);

	/**
	 * @brief	Reference to parent in the scene graph Is equal to null for the root of the scene
	 * 			graph (the game)
	 */
	SceneGraphNode * parent = nullptr;

}; // end SceneNode class

