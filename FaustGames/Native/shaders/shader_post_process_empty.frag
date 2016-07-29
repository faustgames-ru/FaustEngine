#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;

varying vec2 _textureCoords;

void main()
{
	gl_FragColor = texture2D(texture, _textureCoords);
}