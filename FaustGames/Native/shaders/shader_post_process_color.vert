#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec4 color;

varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	gl_Position = vec4(position, 1.0);
	_textureCoords = textureCoords;
	_color = color;
}