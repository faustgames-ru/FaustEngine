#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D paintmask;
varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 mask = texture2D(paintmask, _textureCoords);	
	gl_FragColor = mix(texture2D(texture, _textureCoords), _color, mask.r);
}