#version 330 core

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position.xy, 0.999f, 1.0f);
}