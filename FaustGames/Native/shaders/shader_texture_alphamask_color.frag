#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap; //alphamask

varying vec4 _color;
varying vec2 _textureCoords;

void main()
{
	vec4 color = texture2D(texture, _textureCoords);
	vec4 maskColor = texture2D(lightmap, _textureCoords) * _color;
	color *= maskColor.a;
	float mixLevel = color.a * 0.8;
	color.a = maskColor.a;
	gl_FragColor = mix(maskColor, color, mixLevel); 
}