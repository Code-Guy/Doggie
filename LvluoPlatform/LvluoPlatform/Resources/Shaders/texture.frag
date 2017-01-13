#version 330 core

in vec2 fragTexCoord;

out vec4 fragColor;

uniform sampler2D diffuseTexture;
uniform vec3 colorID;
uniform bool pickOn;
uniform bool blendOn;
uniform bool inverse = false;

const float MaxDiscardAlpha = 0.001f;

void main()
{
	vec2 adjustedFragTexCoord = fragTexCoord;
	if (inverse)
	{
		adjustedFragTexCoord.y = 1.0f - adjustedFragTexCoord.y;
	}
	float blend = blendOn ? 0.5f : 1.0f;
	fragColor = texture(diffuseTexture, adjustedFragTexCoord);
	fragColor.a *= blend;
	if (pickOn)
	{
		if (fragColor.a < MaxDiscardAlpha)
		{
			discard;
		}
		else
		{
			fragColor = vec4(colorID, 1.0f);
		}
	}
}