#version 130

in vec3 texCoord;
out vec4 gl_FragColor;
uniform samplerCube Texture0;

// Cube map access
void main()
{
	gl_FragColor = texture(Texture0, texCoord);
}
