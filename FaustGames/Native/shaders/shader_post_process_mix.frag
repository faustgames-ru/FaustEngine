#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;
varying vec2 _textureCoords;

void main()
{
	vec4 ca = texture2D(texture, _textureCoords);
	vec4 cb = texture2D(lightmap, _textureCoords);
	gl_FragColor = mix(ca, cb, 0.25);
}