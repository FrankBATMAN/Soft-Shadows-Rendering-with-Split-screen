#version 330

layout (location = 0) in vec3 VertexPosition;

out vec4 Color;

void main()
{
	gl_Position = vec4(VertexPosition, 1.0);
	Color = vec4(gl_Position) * 0.5 + 0.5;
}