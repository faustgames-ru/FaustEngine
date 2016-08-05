#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;

varying vec4 _color;
varying vec2 _textureCoords;
varying vec2 _lightmapCoords;
varying float fogFactor;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	vec4 lightColor = texture2D(lightmap, _lightmapCoords);
	//textureColor = vec4(mix(textureColor.xyz, lightColor.xyz, 0.75), textureColor.a)*_color;
	float a = clamp(textureColor.a+0.5, 0.0, 1.0);
	textureColor = vec4(lightColor.xyz*a, a)*_color;
	//textureColor.xyz = mix(textureColor.xyz, vec3(1.0,1.0,1.0), fogFactor);
	gl_FragColor = textureColor;
}