#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;
uniform sampler2D depthmap;

varying vec2 _textureCoords;

float LinearizeDepth(float z)
{
  float n = 1000.0;
  float f = 50000.0;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main()
{
	float d = texture2D(depthmap, _textureCoords).r;
	vec4 ca = texture2D(texture, _textureCoords);
	vec4 cb = texture2D(lightmap, _textureCoords);
	gl_FragColor = mix(ca, cb, clamp(LinearizeDepth(d), 0.0, 1.0));
}