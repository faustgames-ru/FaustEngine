#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform vec3 fogColor;
varying vec4 _color;
varying vec2 _textureCoords;
varying float vertexFogAmount;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	vec3 hsv = rgb2hsv(textureColor.rgb);
	hsv.x =fract(hsv.r + (_color.r - 0.5)*2.0);
	hsv.yz = clamp(hsv.gb + (_color.gb - 0.5)*2.0, 0.0, 1.0);
	textureColor.rgb = hsv2rgb(hsv);
	textureColor.a *= _color.a;	
	textureColor.xyz = mix(textureColor.xyz, fogColor*textureColor.a, vertexFogAmount);
	gl_FragColor = textureColor;		
}