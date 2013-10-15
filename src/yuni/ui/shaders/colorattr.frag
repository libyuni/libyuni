#version 130

in vec4 color;
out vec4 gl_FragColor;

// Use color passed as an attribute
void main()
{
	gl_FragColor = color;
}
