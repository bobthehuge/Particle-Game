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
GLuint program;
GLuint vert;
GLuint frag;

void bglOnLoad()
{
}

void bglOnUpdate()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    /*glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
}

void bglOnUnload()
{}

int main(void)
{   
    float vertices[] = {
        -1.0f, 1.0f, // Top-left
        1.0f, 1.0f, // Top-right
        1.0f, -1.0f, // Bottom-right
        -1.0f, -1.0f, // Bottom-left
    };

    GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
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
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glUseProgram(program);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    bglRunLoop();

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    return 0;
}
