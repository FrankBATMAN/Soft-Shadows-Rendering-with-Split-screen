#version 330
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

uniform mat4 uM;
uniform mat4 uVP;
uniform mat4 uLightSpaceVP;

out Output
{
	vec3 FragmentPosition;
	vec4 FragmentLightPosition;
	vec3 Normal;
	vec2 TexCoords;
} _output;

void main()
{
	gl_Position = uVP * uM * vec4(VertexPosition, 1.0f);

	_output.FragmentPosition = vec3(uM * vec4(VertexPosition, 1.0f));
	_output.FragmentLightPosition = uLightSpaceVP * uM * vec4(VertexPosition, 1.0f);
	_output.Normal = transpose(inverse(mat3(uM))) * VertexNormal;
	_output.TexCoords = VertexTexCoords;
}