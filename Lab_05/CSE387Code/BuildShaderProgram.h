#pragma once

#include "MathLibsConstsFuncs.h"

typedef struct {
	GLenum       type;
	const char*  filename;
	GLuint       shader;
} ShaderInfo;

GLuint BuildShaderProgram(ShaderInfo* shaders);

const GLchar* ReadShader(const char* filename);

