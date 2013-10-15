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
