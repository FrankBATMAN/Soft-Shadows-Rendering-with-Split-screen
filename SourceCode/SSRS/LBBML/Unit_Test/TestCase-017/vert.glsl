#version 330 core
layout(location = 0) in vec3 Position;

uniform mat4 uVP;
uniform mat4 uM;

out vec4 _Color;

void main()
{
	gl_Position = uVP * uM * vec4(Position, 1.0);
	_Color = vec4(clamp(Position, 0.0, 1.0), 1.0);

}