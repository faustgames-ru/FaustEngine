#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform mat4 projection;

attribute vec3 position;
attribute vec3 normal;

varying vec3 _normal;

void main()
{
	gl_Position = projection * vec4(position, 1.0);
	_normal = normal;
}