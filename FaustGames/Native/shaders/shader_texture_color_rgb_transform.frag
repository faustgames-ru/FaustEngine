#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform mat3 colorTransform;
uniform sampler2D texture;
varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 color = texture2D(texture, _textureCoords)*_color;
	color.rgb = abs(color.rgb*colorTransform);
	gl_FragColor = color;
}