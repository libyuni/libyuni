#version 130

in vec2 attrVertex;
out vec2 texCoord;

// For 2D post shaders, texture coordinates are calculated by transforming vertex position
// from [-1.0,1.0] to [0.0,1.0]
void main()
{
	gl_Position = vec4(attrVertex, 0.0f, 1.0f);
	texCoord = (attrVertex + 1.0) / 2.0;
}
