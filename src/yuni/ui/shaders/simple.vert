#version 130

in vec3 attrVertex;
in vec3 attrNormal;
in vec2 attrTexCoord;
out vec3 vertex;
out vec3 normal;
out vec2 texCoord;

// Simple vertex shader : transform vertices, propagate position, texture coordinates and normals
void main(void)
{
	vertex = vec3(gl_ModelViewMatrix * vec4(attrVertex, 1.0f));
	normal = normalize(gl_NormalMatrix * attrNormal);
	texCoord = attrTexCoord;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
}
