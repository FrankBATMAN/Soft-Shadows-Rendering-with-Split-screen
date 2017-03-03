#version 330
layout(location = 0) out vec4 FragColor;

in vec2 _TexCoords;
uniform sampler2D uImage;

void main()
{
	FragColor = texture(uImage, _TexCoords);
}