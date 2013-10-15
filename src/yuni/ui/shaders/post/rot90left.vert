#version 130

in vec2 attrVertex;
out vec2 texCoord;

// 90 degree rotation to the left, texture will be stretched to fit
void main()
{
	gl_Position = vec4(-attrVertex.y, attrVertex.x, 0.0f, 1.0f);
	texCoord = (attrVertex + 1.0) / 2.0;
}
