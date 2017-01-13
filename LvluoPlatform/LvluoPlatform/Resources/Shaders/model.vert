#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 fragNormal;
out vec3 fragPosition;
out vec2 fragTexCoord;
out mat3 tbn;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(position, 1.0f);
    fragNormal = mat3(model) * normal;
    fragPosition = vec3(model * vec4(position, 1.0f));
    fragTexCoord = texCoord;

    vec3 t = normalize(mat3(model) * tangent);
    vec3 b = normalize(mat3(model) * bitangent);
    vec3 n = normalize(fragNormal);

    if(dot(cross(n, t), b) < 0)
    {
    	t *= -1;
    }

    tbn = mat3(t, b, n);
}