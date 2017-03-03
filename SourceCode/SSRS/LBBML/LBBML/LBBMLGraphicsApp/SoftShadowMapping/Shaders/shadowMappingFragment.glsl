#version 330
layout (location = 0) out vec4 FragColor;

in Output
{
	vec3 FragmentPosition;
	vec4 FragmentLightPosition;
	vec3 Normal;
	vec2 TexCoords;
} _input;

uniform float uWidth;
uniform float uHeight;

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;

uniform sampler2D uShadowMap;
uniform sampler2D uLightCenterDepthMap;

float calculateShadowValue(vec4 vFragmentLightPosition)
{
	vec3 ProjectionCoords = _input.FragmentLightPosition.xyz / vFragmentLightPosition.w;
	if(ProjectionCoords.z > 1.0)
		ProjectionCoords.z = 1.0;
	ProjectionCoords = ProjectionCoords * 0.5 + vec3(0.5);
	float currentDepth = ProjectionCoords.z;
	float closestDepth = texture(uLightCenterDepthMap, ProjectionCoords.xy).r;
	return currentDepth - 0.000005 > closestDepth ? 1.0 : 0.0;
}

void main()
{
	vec3 Color = vec3(0.5, 0.5, 0.5);
	vec3 Normal = normalize(_input.Normal);

	vec3 LightColor = vec3(1.0);
	vec3 AmbientStrength = 0.15 * LightColor;

	vec3 LightDir = normalize(uLightPosition - _input.FragmentPosition);
	float Diffuse = max(dot(LightDir, Normal), 0.0);
	vec3 DiffuseStrength = Diffuse * LightColor;

	vec3 CameraDir = normalize(uCameraPosition - _input.FragmentPosition);
	vec3 HalfwayDir = normalize(LightDir + CameraDir);
	float Specular = pow(max(dot(Normal, HalfwayDir), 0.0), 64.0);
	vec3 SpecularStrength = Specular * LightColor;

	//float Shadow = calculateShadowValue(_input.FragmentLightPosition);
	//if(Shadow == 1.0)
	float Shadow;
	if (calculateShadowValue(_input.FragmentLightPosition) == 1.0)
	{
		Shadow =  texture(uShadowMap, gl_FragCoord.xy/vec2(uWidth, uHeight)).r;
	}
	else
	{
		Shadow = 0.0;
	}
	vec3 Light = (AmbientStrength + (1.0 - Shadow) * (DiffuseStrength + SpecularStrength)) * Color;
	FragColor = vec4(Light, 1.0);
}