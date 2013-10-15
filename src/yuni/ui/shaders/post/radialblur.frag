#version 130

in vec2 texCoord;
uniform sampler2D Texture0;

const float sampleDist = 1.0;
const float sampleStrength = 2.2;
const float samples[10] = float[](-0.08, -0.05, -0.03, -0.02, -0.01, 0.01, 0.02, 0.03, 0.05, 0.08);

void main(void)
{
	vec2 uv = texCoord;
    vec2 dir = 0.5 - uv;
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
    dir = dir / dist;

    vec4 color = texture2D(Texture0, uv);
    vec4 sum = color;

    for (int i = 0; i < 10; i++)
        sum += texture2D(Texture0, uv + dir * samples[i] * sampleDist);

    sum *= 1.0 / 11.0;
    float t = dist * sampleStrength;
    t = clamp(t, 0.0, 1.0);

    gl_FragColor = mix(color, sum, t);
}
