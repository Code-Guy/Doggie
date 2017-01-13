#version 330 core

uniform vec3 color;
uniform vec3 colorID;
uniform bool pickOn;
uniform bool blendOn;

out vec4 fragColor;

void main()
{
	if (pickOn)
	{
		fragColor = vec4(colorID, 1.0f);
	}
	else
	{
		fragColor = vec4(color, blendOn ? 0.5f : 1.0f);
	}
}
