#version 330
layout (location = 0) out float RValue;

in vec4 _FragPosition;

void main()
{
	RValue = 0.8;
	//RValue = _FragPosition.x;
}