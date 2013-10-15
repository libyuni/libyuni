#version 130

const uint MAX_LIGHTS = 4u;

in vec3 vertex;
in vec3 normal;
in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;
uniform uint LightCount;
uniform vec4 LightPosition[MAX_LIGHTS];
uniform vec4 LightDiffuse[MAX_LIGHTS];
uniform vec4 LightAmbient[MAX_LIGHTS];
uniform vec4 LightSpecular[MAX_LIGHTS];
uniform vec4 MaterialDiffuse;// = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 MaterialAmbient;// = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 MaterialSpecular;// = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform vec4 MaterialEmission;// = vec4(0.0f, 0.0f, 0.0f, 1.0f);
uniform float MaterialShininess;


// Phong shading with several lights
void main()
{
	vec3 eye = normalize(-vertex); // we are in Eye Coordinates, so EyePos is (0,0,0)
	vec4 texColor = vec4(texture2D(Texture0, texCoord).rgb, 1.0);

	vec4 matColor = vec4(0, 0, 0, 0);
	for (uint i = 0u; i < LightCount; ++i)
	{
		vec3 L = normalize(vec3(gl_ModelViewMatrix * vec4(LightPosition[i].xyz, 0.0f)));
		vec3 R = normalize(-reflect(L, normal));

		// calculate Ambient Term:
		matColor += LightAmbient[i] * MaterialAmbient;

		// calculate Diffuse Term:
		vec4 diff = LightDiffuse[i] * texColor * max(-dot(normal,L), 0.0);
		//diff = clamp(diff, 0.0, 1.0);
		matColor += diff;

		// calculate Specular Term:
		//vec4 spec = LightSpecular[i] * MaterialSpecular * pow(max(dot(R, eye), 0.0), 0.3 * MaterialShininess);
		vec4 spec = LightSpecular[i] * MaterialSpecular * max(pow(-dot(R, eye), MaterialShininess), 0.0);
		//spec = clamp(spec, 0.0, 1.0);
		matColor += spec;
	}
	// write Total Color:
	gl_FragColor = matColor;
}
