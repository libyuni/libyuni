#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

// Use directly the texture value, no lighting
void main()
{
	gl_FragColor = texture(Texture0, texCoord);
}
