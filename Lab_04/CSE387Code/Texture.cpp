#include "Texture.h"
#include "FreeImage.h"
#include <GL/glew.h>
#include <iostream>

GLuint texNames[3] = { 0 };

bool Texture::load(const std::string& textureFileName) {
	// Analyze the bitmap signature to determine the file type
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(textureFileName.c_str(), 0);

	// Read the bitmap from the file
	FIBITMAP* image = FreeImage_Load(format, textureFileName.c_str());

	// Convert the bitmap to 32 bits
	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image); FreeImage_Unload(temp);

	// Get the dimensions of the bitmap
	int width = FreeImage_GetWidth(image);
	int height = FreeImage_GetHeight(image);

	// Check bitmap parameters to determine is a valid image was loaded
	if (image == NULL || width == 0 || height == 0) {
		std::cerr << "ERROR: Unable to load " << textureFileName << "!" << std::endl;
		return -1;

	}// Create a pointer to the bitmap data with the proper type
	GLubyte* texture = new GLubyte[4 * width * height];
	char* texels = (char*)FreeImage_GetBits(image);

	//FreeImage loads in BGR format. Swap some bytes to get into RGB
	for (int j = 0; j < width * height; j++) {
		texture[j * 4 + 0] = texels[j * 4 + 2];
		texture[j * 4 + 1] = texels[j * 4 + 1];
		texture[j * 4 + 2] = texels[j * 4 + 0];
		texture[j * 4 + 3] = texels[j * 4 + 3];
	}

	glGenTextures(3, texNames);

	GLuint diffuseTexture = texNames[0];
	GLuint specularTexture = texNames[1];
	GLuint normalTexture = texNames[2];

	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//glBindTexture(GL_TEXTURE_2D, texNames[1]);
	//glBindTexture(GL_TEXTURE_2D, texNames[2]);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		(GLvoid*)texture
	);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	FreeImage_Unload(image);
}

void Texture::setActive() {
	glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//glBindTexture(GL_TEXTURE_2D, texNames[1]);
	//glBindTexture(GL_TEXTURE_2D, texNames[2]);
}

void Texture::deactivate() {
	glBindTexture(GL_TEXTURE_2D, 0);
}