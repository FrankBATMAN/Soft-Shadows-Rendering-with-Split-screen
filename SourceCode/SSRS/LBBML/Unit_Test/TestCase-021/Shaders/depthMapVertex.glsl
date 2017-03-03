#version 330 core
layout (location = 0) in vec3 VertexPosition;

uniform mat4 uVP;
uniform mat4 uM;

void main()
{
	gl_Position = uVP * uM * vec4(VertexPosition, 1.0f);
}