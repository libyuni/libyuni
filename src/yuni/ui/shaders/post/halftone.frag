#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

const float steps = 128.0;
const float dotsize = 1.0 / steps ;
const float half_step = dotsize / 2.0;

void main(void)
{
    vec2 center = texCoord - vec2(mod(texCoord.x, dotsize), mod(texCoord.y, dotsize)) + half_step;
    vec4 pel = texture2D(Texture0, center);
    float size = length(pel);

    if (distance(texCoord,center) <= dotsize * size / 4.0)
	{
      gl_FragColor = pel;
    }
	else
	{
      gl_FragColor = vec4(0.0,0.0,0.0,0.0);
    };
}
