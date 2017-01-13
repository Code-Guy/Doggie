#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 mv;

void main()
{
    gl_Position = mv * vec4(position, 1.0);
}