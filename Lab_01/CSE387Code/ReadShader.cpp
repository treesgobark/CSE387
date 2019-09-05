#include "ReadShader.h"
#include <cstdlib>


// Reads in the source code of a shader program.  
const GLchar* ReadShader(const char* filename)
{
#ifdef WIN32
	FILE* infile;
	fopen_s(&infile, filename, "rb");
#else
	FILE* infile;

	errno_t err = fopen_s( &infile, filename, "r");
#endif // WIN32

	if (err) {

		std::cerr << "Unable to open file '" << filename << "'" << std::endl;

		return NULL;
	}
	else {

		std::cout << "Reading '" << filename << "' shader source code." << std::endl;
	}

	fseek(infile, 0, SEEK_END);
	int len = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	GLchar* source = new GLchar[len + 1];

	fread(source, 1, len, infile);
	fclose(infile);

	source[len] = 0;

	return const_cast<const GLchar*>(source);

} // end ReadShader

