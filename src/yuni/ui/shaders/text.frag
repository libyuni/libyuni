#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform vec4 TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

// Freetype with normal render mode generates alpha-only bitmaps, stored as GL_R textures
// This shader displays them with the proper color.
void main(void)
{
	vec4 material = texture(Texture0, texCoord);
	vec4 alpha = texture(Texture1, texCoord);

	gl_FragColor = vec4(TextColor.r, TextColor.g, TextColor.b, alpha.r);
	// Keep the given color's alpha, and multiply by the glyph's alpha
	// gl_FragColor = vec4(TextColor.r * material.r, TextColor.g * material.g,
	// 					TextColor.b * material.b, TextColor.a * material.a * alpha.r);
}
