#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;

uniform mat4 uVP;
uniform mat4 uM;

out vec4 _Color;

void main()
{
	gl_Position = uVP * uM * vec4(Position, 1.0);
}