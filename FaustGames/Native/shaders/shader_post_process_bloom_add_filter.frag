#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;
varying vec2 _textureCoords;

float overlay(float a, float b)
{
	return 1.0 - 2.0 * (1.0-a)*(1.0-b);
}

void main()
{
	vec4 ca = texture2D(texture, _textureCoords);
	vec4 cb = texture2D(lightmap, _textureCoords);
	vec4 res =1.0 - (1.0 - ca) * (1.0 - cb);
	res.a = 1.0;
	//res = ca + cb * 0.0;
	gl_FragColor = res;
}