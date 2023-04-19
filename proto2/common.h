#ifndef COMMON
#define COMMON 1

#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <float.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define PI 4.0f * atan(1.0f);

typedef unsigned char byte;
typedef unsigned int uint;
extern byte Board[];

typedef struct vec2
{
    int x;
    int y;
} vec2;

typedef struct Cell
{
    uint color;
    vec2 pos;
    float angle;
} Cell;

void GameInit(void);
void GameUpdate(double deltaTime);
void GameUnload(void);

GLFWwindow* WindowCreate();

GLuint NewShader(GLenum shaderType, const char*);
GLuint NewTexture(unsigned char*);

#endif /* ! */
