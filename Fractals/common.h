#ifndef COMMON
#define COMMON 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*typedef unsigned char byte;
extern byte CellBoard[];

void GameInit(void);
void GameUpdate(void);

GLFWwindow* WindowCreate();*/

GLuint NewShader(GLenum shaderType, const char*);
GLuint NewTexture(unsigned char*);

#endif /* ! */
