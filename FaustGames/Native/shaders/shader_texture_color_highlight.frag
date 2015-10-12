#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform float time;

varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords)*_color;	
	float level = abs(1.0 - mod(time, 2.0)) * textureColor.a;
	vec4 highlightColor = vec4(1.0, 0.9, 0.3, 1.0);
	gl_FragColor = mix(textureColor, highlightColor, level * 0.7);
}