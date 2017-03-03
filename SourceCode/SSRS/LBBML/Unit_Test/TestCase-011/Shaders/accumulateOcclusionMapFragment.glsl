#version 330 
#extension GL_EXT_texture_array : enable
 
layout (location = 0) out float OverAllOcclusionShadow;

in vec2 _TexCoords;

uniform float uSize;
uniform sampler2DArray uSubOcclusionMaps;

void main()
{
	vec3 TexArrayCoord = vec3(_TexCoords.x, _TexCoords.y, 0);

	OverAllOcclusionShadow = 0.0;
	float SubValue;

	for (int i = 0; i < uSize; i++)
	{
		SubValue = texture(uSubOcclusionMaps, TexArrayCoord).r;
		OverAllOcclusionShadow += SubValue / uSize;

		TexArrayCoord.z += 1;
	}
}
