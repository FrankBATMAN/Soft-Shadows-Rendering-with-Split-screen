#version 330

layout (location = 0) in vec3 VertexPosition;

out vec4 ClipPosition;
flat out vec4 SubViewports;
flat out vec2 ScreenResolution;

uniform mat4 uViewMat;
uniform mat4 uProjectionMat;
uniform mat4 uM;

void main()
{
	gl_Position  = uProjectionMat * uViewMat * uM * vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
	ClipPosition = gl_Position;
}