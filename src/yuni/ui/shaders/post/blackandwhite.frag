#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

void main(void)
{
    vec4 tc = texture2D(Texture0, texCoord);
    float color = 0.3 * tc.r + 0.59 * tc.g + 0.11 * tc.b;
	// Either black or white
    color = color < 0.5 ? 0.0 : 1.0;
    gl_FragColor = vec4(color, color, color, tc.a);
}

