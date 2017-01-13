#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoord;
in mat3 tbn;

out vec4 fragColor;

struct Material
{
	vec4 diffuseColor;
	vec4 specularColor;
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;
	bool useDiffuseTexture;
	bool useSpecularTexture;
	bool useNormalTexture;

	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
    float linear;
    float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float innerCutoff;
	float outerCutoff;

	float constant;
    float linear;
    float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

const int PointLightNum = 3;

uniform vec3 cameraPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[PointLightNum];
uniform SpotLight spotLight;
uniform vec3 colorID;
uniform bool pickOn;
uniform bool blendOn;

vec4 DiffuseColor, SpecularColor;

vec4 calcDirectionalLight(DirectionalLight light, vec3 n, vec3 v)
{
	vec3 l = normalize(-light.direction);

	// ambient shading
	vec4 ambient = vec4(light.ambient, 1.0) * DiffuseColor;

	// diffuse shading
	float diffuseStrength = max(dot(n, l), 0.0);
	vec4 diffuse = vec4(light.diffuse, 1.0) * DiffuseColor * diffuseStrength;

	// specular shading
	vec3 h = normalize(l + v);
	float specularStrength = pow(max(dot(n, h), 0.0), material.shininess);
	vec4 specular = vec4(light.specular, 1.0) * SpecularColor * specularStrength;

	return ambient + diffuse + specular;
}

vec4 calcPointLight(PointLight light, vec3 n, vec3 v, vec3 fragPos)
{
	vec3 l = normalize(light.position - fragPos);

	// Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// ambient shading
	vec4 ambient = vec4(light.ambient, 1.0) * DiffuseColor;

	// diffuse shading
	float diffuseStrength = max(dot(n, l), 0.0);
	vec4 diffuse = vec4(light.diffuse, 1.0) * DiffuseColor * diffuseStrength;

	// specular shading
	vec3 h = normalize(l + v);
	float specularStrength = pow(max(dot(n, h), 0.0), material.shininess);
	vec4 specular = vec4(light.specular, 1.0) * SpecularColor * specularStrength;

	return (ambient + diffuse + specular) * attenuation;
}

vec4 calcSpotLight(SpotLight light, vec3 n, vec3 v, vec3 fragPos)
{
	vec3 l = normalize(light.position - fragPos);
	float theta = dot(l, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	// Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// ambient shading
	vec4 ambient = vec4(light.ambient, 1.0) * DiffuseColor;

	// diffuse shading
	float diffuseStrength = max(dot(n, l), 0.0);
	vec4 diffuse = vec4(light.diffuse, 1.0) * DiffuseColor * diffuseStrength;

	// specular shading
	vec3 h = normalize(l + v);
	float specularStrength = pow(max(dot(n, h), 0.0), material.shininess);
	vec4 specular = vec4(light.specular, 1.0) * SpecularColor * specularStrength;

	return (ambient + diffuse + specular) * attenuation * intensity;
}

void main()
{
	if (pickOn)
	{
		fragColor = vec4(colorID, 1.0f);
	}
	else
	{
		DiffuseColor = material.useDiffuseTexture ? texture(material.diffuseTexture, fragTexCoord) : material.diffuseColor;
		SpecularColor = material.useSpecularTexture ? texture(material.specularTexture, fragTexCoord) : material.specularColor;

		vec3 n = normalize(fragNormal);
		vec3 v = normalize(cameraPosition - fragPosition);

		if(material.useNormalTexture)
		{
			vec3 texNormal = texture(material.normalTexture, fragTexCoord).rgb;
			texNormal = texNormal * 2.0 - 1.0;
			n = normalize(tbn * texNormal);
		}

		// directional light
		vec4 ret = calcDirectionalLight(directionalLight, n, v);

		// point lights
		for(int i=0; i<PointLightNum; i++)
		{
			ret += calcPointLight(pointLights[i], n, v, fragPosition);
		}

		// spot light
		ret += calcSpotLight(spotLight, n, v, fragPosition);
		
		float blend = blendOn ? 0.5f : 1.0f;
		fragColor = vec4(ret.xyz, DiffuseColor.a * blend);
	}
}