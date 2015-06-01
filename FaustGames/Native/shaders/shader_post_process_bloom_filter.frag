#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform float threshold;
uniform float alpha;

varying vec2 _textureCoords;

void main()
{
	vec4 color = texture2D(texture, _textureCoords);
	color.rgb = (color.rgb - threshold) * alpha / (1.0-threshold);
	gl_FragColor = color;
}