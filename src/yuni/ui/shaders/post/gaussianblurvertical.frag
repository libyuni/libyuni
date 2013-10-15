#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0; // 0
uniform float Width; // render target width
uniform float Height; // render target height

const float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
const float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main()
{
	vec2 uv = texCoord;
	vec4 tc = texture2D(Texture0, uv) * weight[0];
	for (int i = 1; i < 3; i++)
	{
		tc += texture2D(Texture0, uv + vec2(0.0, offset[i]) / Height) * weight[i];
		tc += texture2D(Texture0, uv - vec2(0.0, offset[i]) / Height) * weight[i];
	}
	gl_FragColor = vec4(tc);
}
