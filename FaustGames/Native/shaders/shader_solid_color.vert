#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform mat4 projection;
attribute vec3 position;
attribute vec4 color;

varying vec4 _color;

void main()
{
	gl_Position = projection *  vec4(position, 1.0);
	_color = color;
}