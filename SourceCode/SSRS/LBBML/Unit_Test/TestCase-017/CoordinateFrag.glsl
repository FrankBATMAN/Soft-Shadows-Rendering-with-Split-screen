#version 330 core
layout(location = 0) out vec4 FragColor;

in vec4 _Color;
void main()
{
	FragColor = _Color;
}