#version 130

in vec2 attrVertex;
out vec2 texCoord;

// Flip the image horizontally
void main()
{
	gl_Position = vec4(-attrVertex.x, attrVertex.y, 0.0f, 1.0f);
	texCoord = (attrVertex + 1.0) / 2.0;
}
