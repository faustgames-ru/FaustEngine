#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;

varying vec4 _color;
varying vec2 _textureCoords;
varying vec2 _lightmapCoords;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	vec4 lightColor = texture2D(lightmap, _lightmapCoords);
	gl_FragColor = textureColor*lightColor*_color;
}