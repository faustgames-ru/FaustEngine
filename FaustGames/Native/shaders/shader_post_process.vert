#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

attribute vec3 position;
attribute vec2 textureCoords;

varying vec2 _textureCoords;

void main()
{
	gl_Position = vec4(position, 1.0);
	_textureCoords = textureCoords;
}