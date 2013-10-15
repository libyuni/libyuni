#version 130

in vec2 texCoord[4];
in vec4 color;
out vec4 gl_FragColor;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;

// Multi-texturing
void main()
{
	vec4 val0 = texture2D(Texture0, vec2(texCoord[0]));
	vec4 val1 = texture2D(Texture1, vec2(texCoord[1]));
	vec4 val2 = texture2D(Texture2, vec2(texCoord[2]));
	vec4 val3 = texture2D(Texture3, vec2(texCoord[3]));
	gl_FragColor = color.x * val0 + color.y * val1 + color.z * val2 + color.w * val3;
}
