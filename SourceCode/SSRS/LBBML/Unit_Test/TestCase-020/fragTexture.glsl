#version 330
layout(location = 0) out vec4 FragColor;

uniform sampler2D uTextureR8;

void main()
{
	float Value = texture(uTextureR8, gl_FragCoord.xy/vec2(1024, 768)).r;

	//FragColor = vec4(Value, Value, Value, 1.0);
	//FragColor = vec4(1.0, 0.0, 0.0, 0.0);
	FragColor = texture(uTextureR8, gl_FragCoord.xy/vec2(1024, 768));
}