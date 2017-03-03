#version 330
layout(location = 0) in vec3 VertexPosition;
layout(location = 2) in vec2 TexCoords;

uniform mat4 uMVP;

out vec2 _TexCoords;

void main()
{
	gl_Position = uMVP * vec4(VertexPosition, 1.0);

	_TexCoords = TexCoords;
}