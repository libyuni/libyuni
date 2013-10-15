#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0; // 0

void main()
{
	vec4 tc = texture2D(Texture0, texCoord);
	gl_FragColor = vec4(1.0f - tc.r, 1.0f - tc.g, 1.0f - tc.b, tc.a);
}
