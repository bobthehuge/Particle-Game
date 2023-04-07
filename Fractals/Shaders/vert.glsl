#version 330 core

layout (location = 0) in vec2 position;
uniform float time;

out vec2 coord;
out vec3 inColor;

void main(void)
{
    
    coord = vec2(position.x * 2 - 0.5, position.y);
    inColor = vec3(0.86f, 0.0, 0.0);

    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}
