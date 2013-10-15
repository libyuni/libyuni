#version 130

in vec3 attrVertex;
in vec4 attrColor;
out vec4 color;

// More than minimal vertex shader : transform coordinates only
void main()
{
	color = attrColor;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
}
