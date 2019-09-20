#pragma once
#include <string>
class Texture
{
public:

	Texture() {};
	~Texture() {};

	/**
	 * @fn	bool Texture::Load(const std::string& fileName);
	 *
	 * @brief	Loads a texture image from the specified file name and
	 * 			creates an associated texture object.
	 *
	 * @param	fileName	The name of the file containing the texture image.
	 *
	 * @returns	True if it succeeds, false if it fails.
	 */
	bool load(const std::string& fileName);

	/**
	 * @fn	void Texture::unload();
	 *
	 * @brief	Deletes the texture object associated with this
	 * 			texture.
	 */
	void unload();

	/**
	 * @fn	void Texture::setActive();
	 *
	 * @brief	Binds the texture object prior to use for rendering
	 */
	void setActive();

	/**
	 * @fn	void Texture::deactivate();
	 *
	 * @brief	Unbinds the texture object after use for rendering
	 */
	void deactivate();

	/**
	 * @fn	int Texture::getWidth() const
	 *
	 * @brief	Gets the width in texels of a two dimensional texture
	 *
	 * @returns	The width.
	 */
	int getWidth() const { return width; }

	/**
	 * @fn	int Texture::getHeight() const
	 *
	 * @brief	Gets the height in texels of a two dimensional texture
	 *
	 * @returns	The height.
	 */
	int getHeight() const { return height; }

	/**
	 * @fn	unsigned int Texture::getTextureObject( ) const
	 *
	 * @brief	Gets unsigned integer identifier of the OpenGL texture object.
	 *
	 * @returns	The texture object.
	 */
	unsigned int getTextureObject() const { return textureID; }

protected:

	// OpenGL ID of this texture
	unsigned int textureID = 0;

	// Width/height of the texture
	int width = 0;
	int height = 0;
};
