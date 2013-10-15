#version 130

in vec3 attrVertex;
in vec3 attrNormal;
in vec4 attrColor;
in vec2 attrTexCoord[4];
out vec4 color;
out vec2 texCoord[4];

// Multi-texturing : Multiple textures and multiple texture coordinates (4 textures max)
// We use the color to determine levels of contribution from each texture
void main()
{
	color = attrColor;
	texCoord[0] = attrTexCoord[0];
	texCoord[1] = attrTexCoord[1];
	texCoord[2] = attrTexCoord[2];
	texCoord[3] = attrTexCoord[3];
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1);
}
