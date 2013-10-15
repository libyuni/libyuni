#version 130

in vec3 attrVertex;

// Color picking
void main()
{
	gl_Position = gl_ModelViewMatrix * vec4(attrVertex, 1.0f);
}
