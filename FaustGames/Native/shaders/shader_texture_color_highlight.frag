#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform float time;
uniform vec3 highlightColor;

varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);	
	textureColor.xyz *= textureColor.a;
	textureColor *= _color;
	float level = abs(1.0 - mod(time, 2.0)) * textureColor.a;
	vec4 highlightColor4 = vec4(highlightColor.x, highlightColor.y, highlightColor.z, 1.0);
	gl_FragColor = mix(textureColor, highlightColor4, level * 0.7);
}