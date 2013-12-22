#pragma once

// This file contains necessary GLSL shaders for internal use

namespace Yuni
{

namespace Gfx3D
{

	/////////////// VERTEX SHADERS


	static const char vsTransformOnly[] =
		R"(
#version 130

in vec3 attrVertex;

// More than minimal vertex shader : transform coordinates only
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
}
		)";


	// Minimal vertex shader : transform coordinates and propagate texture coordinates
	static const char vsMinimal[] =
		R"(
#version 130

in vec3 attrVertex;
in vec2 attrTexCoord;
out vec2 texCoord;

// Minimal vertex shader : transform coordinates and propagate texture coordinates
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
	texCoord = attrTexCoord;
}
		)";


	// Pass the color as attribute
	static const char vsColorAttr[] =
		R"(
#version 130

in vec3 attrVertex;
in vec4 attrColor;
out vec4 color;

// Pass the color as attribute
void main()
{
	color = attrColor;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
}
		)";


	// Phong shading
	static const char vsPhong[] =
		R"(
#version 130

const uint MAX_LIGHTS = 4u;

in vec3 attrVertex;
in vec3 attrNormal;
in vec2 attrTexCoord;
out vec3 vertex;
out vec3 normal;
out vec2 texCoord;
uniform uint LightCount;
uniform vec4 LightPosition[MAX_LIGHTS];

const vec3 pos = vec3(-0.5, 0.5, 0.0);

// Vertex shader for Phong shading with several lights
void main(void)
{
	vertex = vec3(gl_ModelViewMatrix * vec4(attrVertex, 1));
	normal = normalize(gl_NormalMatrix * attrNormal);
	texCoord = attrTexCoord;

	// for (uint i = 0u; i < LightCount; ++i)
	// {
	// 	vec3 lightPos = vec3(gl_ModelViewMatrix * LightPosition[i]);
	// 	// Directional
	// 	lightDir[i] = normalize(-lightPos);
	// 	// Omni / Point
	// 	// lightDir[i] = normalize(lightPos - vertex);
	// }
	//lightDir0 = normalize(vec3(gl_ModelViewMatrix * vec4(pos, 0.0)));

	gl_Position = gl_ModelViewProjectionMatrix * vec4(attrVertex, 1.0f);
}
		)";





	/////////////// FRAGMENT SHADERS


	// Use a single color given as uniform
	static const char fsColorUniform[] =
		R"(
#version 130

out vec4 gl_FragColor;
uniform vec4 Color;

// Use a single color given as uniform
void main()
{
	gl_FragColor = Color;
}
		)";


	// Use a single color given as attribute
	static const char fsColorAttr[] =
		R"(
#version 130

in vec4 color;
out vec4 gl_FragColor;

// Use color passed as an attribute
void main()
{
	gl_FragColor = color;
}
		)";


	// Use directly the texture value, no lighting
	static const char fsSimpleTexture[] =
		R"(
#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

// Use directly the texture value, no lighting
void main()
{
	gl_FragColor = texture(Texture0, texCoord);
}
		)";


	// Freetype with normal render mode generates alpha-only bitmaps, stored as GL_R textures
	// This shader displays them with the proper color.
	static const char fsText[] =
		R"(
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
	vec4 alpha = texture(Texture0, texCoord);
	vec4 material = texture(Texture1, texCoord);

	gl_FragColor = vec4(TextColor.r, TextColor.g, TextColor.b, alpha.r);
}
		)";


	// Color picking
	static const char fsPicking[] =
		R"(
#version 410

out vec4 gl_FragColor;

// uint is 32-bit
uniform uint ObjectID = 0u;

// Color picking
void main()
{
	float highest = (ObjectID >> 24u) / 255.0f;
	float high = ((ObjectID >> 16u) & 0xff) / 255.0f;
	float low = ((ObjectID >> 8u) & 0xff) / 255.0f;
	float lowest = (ObjectID & 0xff) / 255.0f;
	gl_FragColor = vec4(highest, high, low, lowest);
}
		)";


	// Phong shading
	static const char fsPhong[] =
		R"(
#version 130

const uint MAX_LIGHTS = 4u;

in vec3 vertex;
in vec3 normal;
in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform uint LightCount;
uniform vec4 LightPosition[MAX_LIGHTS];
uniform vec4 LightDiffuse[MAX_LIGHTS];
uniform vec4 LightAmbient[MAX_LIGHTS];
uniform vec4 LightSpecular[MAX_LIGHTS];
uniform vec4 MaterialDiffuse = vec4(1.0f, 0.0f, 0.0f, 0.0f);
uniform vec4 MaterialAmbient = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 MaterialSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 MaterialEmission = vec4(0.0f, 0.0f, 0.0f, 1.0f);
uniform float MaterialShininess;


// Phong shading with several lights
void main()
{
	vec3 eye = normalize(-vertex); // we are in Eye Coordinates, so EyePos is (0,0,0)
	vec4 texColor = vec4(texture2D(Texture0, texCoord).rgb, 1.0);

	vec4 matColor = vec4(0, 0, 0, 0);
	for (uint i = 0u; i < LightCount && i < MAX_LIGHTS; ++i)
	{
		vec3 L = normalize(vec3(gl_ModelViewMatrix * vec4(LightPosition[i].xyz, 0.0f)));
		vec3 R = normalize(-reflect(L, normal));

		// calculate Ambient Term:
		matColor += LightAmbient[i] * MaterialAmbient;

		// calculate Diffuse Term:
		vec4 diff = LightDiffuse[i] * MaterialDiffuse * texColor * max(-dot(normal,L), 0.0);
		diff = clamp(diff, 0.0, 1.0);
		matColor += diff;

		// calculate Specular Term:
		//vec4 spec = LightSpecular[i] * MaterialSpecular * pow(max(dot(R, eye), 0.0), 0.3 * MaterialShininess);
		vec4 spec = LightSpecular[i] * MaterialSpecular * max(pow(-dot(R, eye), MaterialShininess), 0.0);
		spec = clamp(spec, 0.0, 1.0);
		matColor += spec;
	}
	// write Total Color:
	gl_FragColor = matColor;
}
		)";




} // namespace Gfx3D
} // namespace Yuni
