#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
varying vec2 _textureCoords;

const float from = 0.8;
const float to = 1.0;
const float size = to - from;

void main()
{
	vec4 color = texture2D(texture, _textureCoords);
	//color.rgb = (color.rgb - from) / size;
	gl_FragColor = color;
}