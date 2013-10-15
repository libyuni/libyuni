#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;
uniform float Offset = 0.0f;
const float factor = 4 * 2 * 3.14159;

// Image post-processing
// Wave effect / horizontal sinusoidal displacement
void main()
{
	vec2 uv = vec2(texCoord.x + sin(texCoord.y * factor + Offset) / 100, texCoord.y);
	gl_FragColor = texture2D(Texture0, uv);
}
