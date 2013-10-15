#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

const uint nbTones = 4u;

void main(void)
{
    vec4 tc = texture(Texture0, texCoord);
    float color = 0.3 * tc.r + 0.59 * tc.g + 0.11 * tc.b;
	// Several tones of gray
	color = min(float(floor(color * (nbTones + 1u))) / nbTones, 1.0);
    gl_FragColor = vec4(color, color, color, tc.a);
}

