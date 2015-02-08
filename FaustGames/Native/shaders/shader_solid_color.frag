#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 _color;

void main() 
{
	gl_FragColor = _color;
}