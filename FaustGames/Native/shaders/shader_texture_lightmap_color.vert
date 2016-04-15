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
varying float fogFactor;

float calcFogFactor(float distance)
{
  const float LOG2 = 1.442695;
  const float density = 0.0013;
  float d = max(distance, 0.0);
  float factor =exp2( -density*density*d*d*LOG2 );
  return 0.5 - clamp(factor, 0.0, 0.5);  
}

void main()
{
	gl_Position = projection * vec4(position, 1.0);
	_color = color;
	_textureCoords = textureCoords;
	_lightmapCoords = (gl_Position.xy / gl_Position.w) * vec2(0.5, 0.5) + 0.5;
	fogFactor = calcFogFactor(position.z);
}