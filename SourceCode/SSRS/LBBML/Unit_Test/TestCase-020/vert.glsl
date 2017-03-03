#version 330
layout (location = 0) in vec3 Position;

uniform mat4 uVP;
uniform mat4 uM;

out vec4 _FragPosition;

void main()
{
	gl_Position = uVP * uM * vec4(Position, 1.0);

	_FragPosition = gl_Position;
}