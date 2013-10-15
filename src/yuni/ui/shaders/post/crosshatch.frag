#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

const uint nbTones = 4u;

void main(void)
{
	vec4 tc = texture(Texture0, texCoord);
    float gray = 0.3 * tc.r + 0.59 * tc.g + 0.11 * tc.b;
	// Several tones of gray give different crosshatching patterns
	gl_FragColor = vec4(1.0, 1.0, 1.0, tc.a);
	if (gray < 1.00)
	{
		if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0)
			gl_FragColor = vec4(0.0, 0.0, 0.0, tc.a);
	}
	if (gray < 0.75)
	{
		if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0)
			gl_FragColor = vec4(0.0, 0.0, 0.0, tc.a);
	}
	if (gray < 0.50)
	{
		if (mod(gl_FragCoord.x + gl_FragCoord.y - 5.0, 10.0) == 0.0)
			gl_FragColor = vec4(0.0, 0.0, 0.0, tc.a);
	}
	if (gray < 0.3465)
	{
		if (mod(gl_FragCoord.x - gl_FragCoord.y - 5.0, 10.0) == 0.0)
			gl_FragColor = vec4(0.0, 0.0, 0.0, tc.a);
	}
}
