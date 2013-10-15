#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0; // 0

// RGB to grayscale using GIMP's average method (simple averaging of values)
void main()
{
	vec4 tc = texture2D(Texture0, texCoord);
	float color = (tc.r + tc.g + tc.b) / 3.0f;
	gl_FragColor = vec4(color, color, color, tc.a);
}
