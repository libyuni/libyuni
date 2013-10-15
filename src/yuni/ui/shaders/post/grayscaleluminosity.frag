#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0; // 0

// RGB to grayscale using GIMP's luminosity method / BT709's luma coefficients
void main()
{
	vec4 tc = texture2D(Texture0, texCoord);
	float color = 0.2126f * tc.r + 0.7152f * tc.g + 0.0722f * tc.b;
	gl_FragColor = vec4(color, color, color, tc.a);
}
