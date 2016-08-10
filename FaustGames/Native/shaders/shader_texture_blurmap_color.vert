#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform mat4 projection;
attribute vec3 position;
attribute vec2 textureCoords;
attribute vec4 color;

varying vec4 _color;
varying vec2 _textureCoords;
varying vec2 _lightmapCoords;
varying float depthFactor;

float calcDepthFactor(float distance)
{
  return clamp(distance / 40000.0, 0.0, 0.7); 
}

void main()
{
	gl_Position = projection * vec4(position, 1.0);
	_color = vec4(color.rgb*2.0, color.a);
	_textureCoords = textureCoords;
	_lightmapCoords = (gl_Position.xy / gl_Position.w) * vec2(0.5, 0.5) + 0.5;
	depthFactor = calcDepthFactor(position.z);
}