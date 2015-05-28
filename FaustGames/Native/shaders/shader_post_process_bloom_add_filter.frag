#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;
varying vec2 _textureCoords;

float overlay(float a, float b)
{
	if(a < 0.5)
	{
		return 2.0*a*b;
	}
	else
	{
		return 1.0 - 2.0 * (1.0-a)*(1.0-b);
	}
}

void main()
{
	vec4 ca = texture2D(texture, _textureCoords);
	vec4 cb = texture2D(lightmap, _textureCoords);
	
	gl_FragColor = vec4(
		overlay(ca.r, cb.r),
		overlay(ca.g, cb.g),
		overlay(ca.b, cb.b),
		1.0);
}