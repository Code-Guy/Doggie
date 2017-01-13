#version 330 core

layout(points) in;//输入一个点
layout(triangle_strip, max_vertices = 4) out;//输出一个4个点的triangle strip（四边形）

out vec2 fragTexCoord;

uniform mat4 p;
uniform float size;

void main()
{
	vec4 center = gl_in[0].gl_Position;

	vec4 v0 = vec4(center.xy + vec2(0.707f, 0.707f) * size, center.z, 1.0);	//1象限
    vec4 v1 = vec4(center.xy + vec2(-0.707f, 0.707f) * size, center.z, 1.0);	//2象限
    vec4 v2 = vec4(center.xy + vec2(-0.707f, -0.707f) * size, center.z, 1.0);  //3象限
   	vec4 v3 = vec4(center.xy + vec2(0.707f, -0.707f) * size, center.z, 1.0);  //4象限

   	//0--1--3--2三角形条带规定顺序
	gl_Position = p * v0;
	fragTexCoord = vec2(1, 1);
	EmitVertex();

	gl_Position = p * v1;
	fragTexCoord =  vec2(0, 1);
	EmitVertex();

	gl_Position = p * v3;
	fragTexCoord =  vec2(1, 0);
	EmitVertex();

	gl_Position = p * v2;
	fragTexCoord = vec2(0, 0);
	EmitVertex();

	EndPrimitive();
}
