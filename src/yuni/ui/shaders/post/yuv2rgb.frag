#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform uint PaddedWidth = 1u;
uniform uint Width = 1u; // The only important thing here is the Width / PaddedWidth ratio
uniform sampler2D TextureY;
uniform sampler2D TextureU;
uniform sampler2D TextureV;

void main()
{
	// Correct horizontal texture coordinate to account for padding
	vec2 fixedCoord = vec2(texCoord.s * Width / PaddedWidth, 1.0 - texCoord.t);

	// Sample the texture
	float y = texture2D(TextureY, fixedCoord).r;
	float cb = texture2D(TextureU, fixedCoord).r;
	float cr = texture2D(TextureV, fixedCoord).r;

	y = 1.1643 * (y - 0.0625);
	cb = cb - 0.5;
	cr = cr - 0.5;

	float r = y + 1.5958 * cr;
	float g = y - 0.39173 * cb - 0.81290 * cr;
	float b = y + 2.017 * cb;

	gl_FragColor = vec4(r, g, b, 1.0);
}
