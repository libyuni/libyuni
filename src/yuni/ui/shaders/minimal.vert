#version 130

in vec3 attrVertex;
in vec2 attrTexCoord;
out vec2 texCoord;

// Minimal vertex shader : transform coordinates and propagate texture coordinates
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
	texCoord = attrTexCoord;
}
