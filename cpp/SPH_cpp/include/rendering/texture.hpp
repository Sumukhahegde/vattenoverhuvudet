#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#ifdef __WIN32
#include <GL/glew.h>
#endif

#include "GLFW/glfw3.h"

GLuint makeBO(GLenum type, void* data, GLsizei size, int accessFlags);

GLuint makeTextureBuffer(int w, int h, GLenum format, GLint internalFormat);

GLuint loadTexture(const char *filename);

char* loadFile(char* name);
GLuint genFloatTexture(float *data, int width, int height);

float* loadPGM(const char* fileName, int w, int h);