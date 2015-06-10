#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;

varying vec2 _textureCoords;

void main()
{
	vec4 color = texture2D(texture, _textureCoords);
	//vec2 toneCoords = vec2((floor(color.r * 31.0) * 32.0 + color.g * 31.0)/1023.0, color.b);
	float r = floor(0.0 + color.r * 31.0)/ 32.0;
	float g = (0.0 + color.g * 31.0) / 1024.0;
	float b = color.b;//(0.0 + color.b * 31.0) / 32.0;
	vec2 toneCoords = vec2(r+g, b);
	gl_FragColor = texture2D(lightmap, toneCoords) ;
}