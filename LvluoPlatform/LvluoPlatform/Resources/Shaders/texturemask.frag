#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform vec3 color;
uniform vec3 colorID;
uniform bool pickOn;
uniform bool blendOn;

const float MaxDiscardAlpha = 0.001f;

void main()
{
	float alpha = texture(diffuseTexture, fragTexCoord).a;

	if (pickOn)
	{
		if (alpha < MaxDiscardAlpha)
		{
			discard;
		}
		else
		{
			fragColor = vec4(colorID, 1.0f);
		}
	}
	else
	{
		float blend = blendOn ? 0.5f : 1.0f;
		fragColor = vec4(color, alpha * blend);
	}
}