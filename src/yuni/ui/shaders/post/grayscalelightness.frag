#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0; // 0

// RGB to grayscale using GIMP's lightness method
void main()
{
	vec4 tc = texture2D(Texture0, texCoord);
	float max = max(tc.r, max(tc.g, tc.b));
	float min = min(tc.r, min(tc.g, tc.b));
	float mean = (max + min) / 2.0f;
	gl_FragColor = vec4(mean, mean, mean, tc.a);
}
