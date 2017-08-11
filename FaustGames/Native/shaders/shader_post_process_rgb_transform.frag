#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform mat3 colorTransform;
uniform sampler2D texture;
uniform vec3 colorOffset;

varying vec2 _textureCoords;

void main()
{
	vec4 color = texture2D(texture, _textureCoords);
	color.xyz = abs(clamp(color.xyz - colorOffset, 0.0, 1.0)*colorTransform);
	gl_FragColor = color;
}