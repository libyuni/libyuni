#version 130

in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D Texture0;

const float step_w = 0.0015625;
const float step_h = 0.0027778;

void main(void)
{
	vec2 uv = texCoord;
	vec4 tc = texture2D(Texture0, uv);

    vec3 t1 = texture2D(Texture0, vec2(uv.x - step_w, uv.y - step_h)).bgr;
    vec3 t2 = texture2D(Texture0, vec2(uv.x, uv.y - step_h)).bgr;
    vec3 t3 = texture2D(Texture0, vec2(uv.x + step_w, uv.y - step_h)).bgr;
    vec3 t4 = texture2D(Texture0, vec2(uv.x - step_w, uv.y)).bgr;
    vec3 t5 = tc.bgr;
    vec3 t6 = texture2D(Texture0, vec2(uv.x + step_w, uv.y)).bgr;
    vec3 t7 = texture2D(Texture0, vec2(uv.x - step_w, uv.y + step_h)).bgr;
    vec3 t8 = texture2D(Texture0, vec2(uv.x, uv.y + step_h)).bgr;
    vec3 t9 = texture2D(Texture0, vec2(uv.x + step_w, uv.y + step_h)).bgr;

    vec3 rr = -4.0 * t1 - 4.0 * t2 - 4.0 * t4 + 12.0 * t5;
    float y = (rr.r + rr.g + rr.b) / 3.0;

    gl_FragColor.a = tc.a;
    gl_FragColor.rgb = vec3(y, y, y) + 0.3;
}
