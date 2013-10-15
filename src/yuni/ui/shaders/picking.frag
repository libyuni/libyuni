#version 130

out vec4 gl_FragColor;

uniform uint ObjectID = 0u;

// Color picking
void main()
{
	gl_FragColor = uvec4(ObjectID, 0, 0, 1);
}
