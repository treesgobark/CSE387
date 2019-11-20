#pragma once
#include "Component.h"
#include "SharedGeneralLighting.h"

/**
 * @class	LightComponent
 *
 * @brief	A light component. Base class for AmbientLightComponent, DirectionalLightComponent,
 * 			PositionalLightComponent, and SpotLightComponent classes.
 */
class LightComponent :public Component
{
public:

	/**
	 * @fn	LightComponent::LightComponent(class GameObject* owner, lightSource id, bool enabled);
	 *
	 * @brief	Constructor
	 *
	 * @param 		  	id	   	The light identifier.
	 * @param 		  	enabled	True to enable, false to disable.
	 */
	LightComponent(lightSource id, bool enabled);

	/**
	 * @fn	void LightComponent::setEnable();
	 *
	 * @brief	Turns the light source on.
	 */
	void setEnable();

	/**
	 * @fn	void LightComponent::setDisable();
	 *
	 * @brief	Turns the light source off.
	 */
	void setDisable();

	/**
	 * @fn	void LightComponent::setAmbientColor(vec4 newAmb);
	 *
	 * @brief	Sets ambient color of the light source.
	 *
	 * @param	newAmb	The new ambient color of the light source.
	 */
	void setAmbientColor(vec4 newAmb);

	/**
	 * @fn	void LightComponent::setDiffuseColor(vec4 newDif);
	 *
	 * @brief	Sets diffuse color of the light source.
	 *
	 * @param	newDif	The new diffuse color of the light source.
	 */
	void setDiffuseColor(vec4 newDif);

	/**
	 * @fn	void LightComponent::setSpecularColor(vec4 newSpec);
	 *
	 * @brief	Sets specular color of the light source.
	 *
	 * @param	newSpec	The new specular color of the light source.
	 */
	void setSpecularColor(vec4 newSpec);

protected:


	/** @brief	The identifier for the light source. 
	i.e. GL_LIGHT_ZERO, GL_LIGHT_ONE, etc.*/
	lightSource id;

}; // end LightComponent

