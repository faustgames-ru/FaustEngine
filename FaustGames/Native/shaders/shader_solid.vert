#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

attribute vec3 position;
uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(position, 1.0);
}