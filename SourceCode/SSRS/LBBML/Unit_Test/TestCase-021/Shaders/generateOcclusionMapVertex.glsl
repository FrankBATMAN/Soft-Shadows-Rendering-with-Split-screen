#version 330
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

uniform mat4 uM;
uniform mat4 uVP;
uniform mat4 uLightSpaceVP;

out vec4 _FragmentLightPosition;

void main()
{
	gl_Position = uVP * uM * vec4(VertexPosition, 1.0f);

	_FragmentLightPosition = uLightSpaceVP * uM * vec4(VertexPosition, 1.0f);
}