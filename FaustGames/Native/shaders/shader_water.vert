#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform mat4 projection;
uniform vec3 cameraPosition;
uniform float time;

uniform float textureVelocity;
uniform float wavesVelocity;
uniform float wavesScale;

const float Pi = 3.14;

attribute vec3 position;
attribute vec2 textureCoords;

varying vec2 _textureCoords0;
varying vec2 _textureCoords1;
varying vec3 _eye;
varying vec2 _screen;
varying float _z;

void main()
{
	vec3 pos = position;
	pos.y = pos.y + sin((pos.x + pos.z - time*wavesVelocity) * Pi)*wavesScale;
	vec4 glp = projection * vec4(pos, 1.0);	
	gl_Position = glp;
	_screen = glp.xy / glp.w;
	_z = glp.z / glp.w;
	_eye = normalize(position - cameraPosition);
	_textureCoords0 = textureCoords + time*textureVelocity;
	_textureCoords1 = textureCoords - time*textureVelocity;
}