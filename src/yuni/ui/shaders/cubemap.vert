#version 130

in vec3 attrVertex;
out vec3 texCoord;

// 3D texture coordinates for a cubemap are actually the vertex' position
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
	texCoord = normalize(attrVertex);
}
