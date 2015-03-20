#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;

varying vec2 _textureCoords;

float LinearizeDepth(float z)
{
  float n = 1000.0;
  float f = 50000.0;
  return (2.0 * n) / (f + n - z * (f - n));	
}
void main()
{
	float d = LinearizeDepth(texture2D(texture, _textureCoords).r);
	gl_FragColor = vec4(d, d, d, 1.0);
}