#version 330
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

uniform mat4 uVP;
uniform mat4 uM;

void main()
{
	gl_Position = uVP * uM * vec4(Position, 1.0);
}