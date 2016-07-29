#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;

varying vec2 _textureCoords;
varying vec4 _color;

void main()
{
	gl_FragColor = vec4(texture2D(texture, _textureCoords).xyz * _color.xyz, _color.a);
}