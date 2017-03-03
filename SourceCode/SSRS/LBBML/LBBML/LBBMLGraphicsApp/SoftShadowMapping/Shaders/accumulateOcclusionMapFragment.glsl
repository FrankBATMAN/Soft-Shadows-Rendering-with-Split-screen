#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 _TexCoords;

uniform float uIndex;
uniform float uSize;

uniform sampler2D uSubOcclusionMap;
uniform sampler2D uSoftShadowMap;

void main()
{
	float SubValue = texture(uSubOcclusionMap, _TexCoords).r;
	float SumValue = texture(uSoftShadowMap, _TexCoords).r;

	FragColor = vec4(SumValue + SubValue / uSize, 0.0, 0.0, 1.0);
}

/*
#version 330 
#extension GL_EXT_texture_array : enable
 
layout (location = 0) out vec4 FragColor;

in vec2 _TexCoords;

uniform float uSize;

uniform sampler2DArray uSubOcclusionMaps;

void main()
{
	vec3 TexArrayCoord = vec3(_TexCoords.x, _TexCoords.y, 0.0f);

	float Shadow = 0.0;
	float SubValue;

	for (int i = 0; i < uSize; i++)
	{
		SubValue = texture(uSubOcclusionMaps, TexArrayCoord).x;
		Shadow += SubValue / uSize;

		TexArrayCoord += vec3(0.0, 0.0, 1.0);
	}

	FragColor = vec4(Shadow, 0.0, 0.0, 1.0);

}
*/