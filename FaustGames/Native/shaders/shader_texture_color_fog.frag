#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
uniform vec3 fogColor;
varying vec4 _color;
varying vec2 _textureCoords;
varying float vertexFogAmount;

void main()
{
	vec4 textureColor = texture2D(texture, _textureCoords);
	textureColor.xyz = clamp(textureColor.xyz, vec3(0), textureColor.aaa);
	textureColor *= _color;
	textureColor.xyz = mix(textureColor.xyz, fogColor*textureColor.a, vertexFogAmount);
	gl_FragColor = textureColor;
}