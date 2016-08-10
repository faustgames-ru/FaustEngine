#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform sampler2D lightmap;

varying vec4 _color;
varying vec2 _textureCoords;
varying vec2 _lightmapCoords;
varying float depthFactor;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	vec3 blurColor = texture2D(lightmap, _lightmapCoords).rgb * textureColor.a;
	textureColor.rgb = mix(textureColor.rgb, blurColor, clamp(depthFactor, 0.0, 1.0));
	gl_FragColor = textureColor;
}