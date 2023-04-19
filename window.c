#include <bobgl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common.h"

int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;
char* WINDOW_TITLE = "test";
int RESIZABLE = 0;
int FULLSCREEN = 1;

GLuint vao;
GLuint tex;
GLuint program;
GLuint vert;
GLuint frag;

const double fpsLimit = 0;
double lastUpdateTime = 0;  // number of seconds since the last loop
double lastFrameTime = 0;

void bglOnLoad()
{
    srand(time(NULL));
    GameInit();
}

void bglOnUpdate()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    double now = glfwGetTime();

    double deltaTime = now - lastFrameTime;

    if (deltaTime >= fpsLimit)
    {
        // draw your frame here
        GameUpdate(deltaTime);

        // only set lastFrameTime when you actually draw something
        lastFrameTime = now;
    }

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, Board);

    lastUpdateTime = now;
}

void bglOnUnload()
{
    glDetachShader(program, vert);
    glDetachShader(program, frag);
    glDeleteTextures(1, &tex);
}

int main(void)
{   
    float vertices[] = {
    //   Position      Texcoords
        -1.0f,  1.0f,  0.0f, 0.0f, // Top-left
         1.0f,  1.0f,  1.0f, 0.0f, // Top-right
         1.0f, -1.0f,  1.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f,  0.0f, 1.0f // Bottom-left
    };

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    bglInit();
    
    vert = NewShader(GL_VERTEX_SHADER, "Shaders/vert.glsl");
    frag = NewShader(GL_FRAGMENT_SHADER, "Shaders/frag.glsl");

    program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
    
    GLint texAttrib = glGetAttribLocation(program, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, 
        (void*)(sizeof(float)*2));

    glUseProgram(program);
    
    tex = NewTexture(Board);
    
    bglRunLoop();
    bglTerminate();

    return 0;
}
