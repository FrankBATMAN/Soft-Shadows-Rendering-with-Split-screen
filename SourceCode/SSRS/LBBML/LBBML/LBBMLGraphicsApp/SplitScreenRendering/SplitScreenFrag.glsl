#version 330

out vec4 FragColor;

in vec4 ClipPosition;

uniform vec4 uSubViewports;
uniform vec2 uScreenResolution;

void main()
{	
	float PositionXLower = uSubViewports.x;
	float PositionXUpper = uSubViewports.z; 

	float PositionYLower = uSubViewports.y;;
	float PositionYUpper = uSubViewports.w;

	vec2 ScreenPosition = (ClipPosition / ClipPosition.w).xy * uScreenResolution / 2.0 +  uScreenResolution / 2.0;
	 
	if (ScreenPosition.x > PositionXLower && ScreenPosition.x < PositionXUpper && ScreenPosition.y > PositionYLower && ScreenPosition.y < PositionYUpper)
	{
		FragColor = vec4(0.5, 0.5, 0.5, 1.0);
	}
	else
	{
		discard;
	}

}