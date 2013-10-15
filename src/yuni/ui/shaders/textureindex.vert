#version 130

in vec3 attrVertex;
in vec3 attrNormal;
in vec2 attrTexCoord;
in float attrTexIndex;
out vec3 normal;
out vec3 vertex;
out vec2 texCoord;
out float texIndex;

// Multiple textures were defined and an index was given for each vertex to choose the proper one
void main()
{
	vertex = vec3(gl_ModelViewMatrix * vec4(attrVertex, 1));
	normal = normalize(gl_NormalMatrix * attrNormal);
	texIndex = attrTexIndex;
	texCoord = attrTexCoord;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1);
}
