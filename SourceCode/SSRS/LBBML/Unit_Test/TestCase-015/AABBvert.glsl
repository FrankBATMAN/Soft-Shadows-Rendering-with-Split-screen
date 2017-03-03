#version 330 core
layout(location = 0) in vec3 Position;

uniform mat4 uVP;

void main()
{
	gl_Position = uVP * vec4(Position, 1.0);
}