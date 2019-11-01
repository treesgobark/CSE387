#include "Texture.h"
#include "FreeImage.h"

// Static variable must be defined outside the declaration
// Map containing all textures that have been loaded
std::unordered_map<std::string, class Texture*> Texture::loadedTextures;

bool Texture::load(const std::string& fileName)
{
	// Analyze the bitmap signature to determine the file type
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);

	// Read the bitmap from the file
	FIBITMAP* image = FreeImage_Load(format, fileName.c_str());

	// Convert the bitmap to 32 bits
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);

	// Get the dimensions of the bitmap
	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);

	// Check bitmap parameters to determine is a valid image was loaded
	if (image == nullptr || width == 0 || height == 0) {
		std::cerr << "ERROR: Unable to load " << fileName << "!" << std::endl;
		return false;
	}

	// Create a pointer to the bitmap data with the proper type
	GLubyte* texture = new GLubyte[4 * width * height];
	char* texels = (char*)FreeImage_GetBits(image);

	//FreeImage loads in BGR format, so you need to swap some bytes.
	for (int j = 0; j < width * height; j++) {
		texture[j * 4 + 0] = texels[j * 4 + 2];
		texture[j * 4 + 1] = texels[j * 4 + 1];
		texture[j * 4 + 2] = texels[j * 4 + 0];
		texture[j * 4 + 3] = texels[j * 4 + 3];
	}

	glGenTextures(1, &this->textureID);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, this->textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);

	std::cout << "Loaded: " << fileName.c_str() << " texture. width " << width << " height " << height << std::endl;

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(image);

	return true;

}


void Texture::unload()
{
	// Remove the Texture object from the Map
	loadedTextures.erase(fileName);

	// Delete the texture object
	glDeleteTextures(1, &textureID);

} // unload


Texture* Texture::GetTexture(const std::string& fileName)
{
	// Pointer to the texture to be loaded or retrieved.
	Texture* texturePtr = nullptr;

	// Search for the texture among those that were previously loaded
	auto iter = loadedTextures.find(fileName);

	// Check if the texture was previously loaded
	if (iter != loadedTextures.end()) {

		std::cout << "Retrieving texture: " << fileName << std::endl;
		texturePtr = iter->second;
	}
	else {

		std::cout << "Loading texture: " << fileName << std::endl;
		texturePtr = new Texture();

		texturePtr->fileName = fileName;

		// Load the texture
		if (texturePtr->load(fileName)) {

			// Add the loaded texture to those that were previously loaded
			loadedTextures.emplace(fileName, texturePtr);
		}
		else {
			delete texturePtr;
			texturePtr = nullptr;
		}
	}

	return texturePtr;

} // end GetTexture


void Texture::unloadTextures()
{
	// Destroy textures
	for (auto i : loadedTextures) {
		i.second->unload();
	}
	loadedTextures.clear();

} // end unloadTextures
