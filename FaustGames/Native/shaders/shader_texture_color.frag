#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	textureColor.xyz = clamp(textureColor.xyz, vec3(0), textureColor.aaa);
	gl_FragColor = textureColor*_color;
}