#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D environment;
uniform sampler2D normalmap;
uniform float normalScaleZ;
uniform float envOffsetY;
uniform float envScaleY;

varying vec2 _textureCoords0;
varying vec2 _textureCoords1;
varying vec3 _eye;

void main()
{
	vec3 normal0 = texture2D(normalmap, _textureCoords0).xyz - 0.5;
	vec3 normal1 = texture2D(normalmap, _textureCoords1).xyz - 0.5;
	vec3 normal = normalize(normal0 + normal1);
	normal.z *= normalScaleZ;
	normal = normalize(normal);
	vec3 env = normalize(reflect (_eye, normal));
	float mixLevel = 1.0 - pow(clamp((dot(_eye, normal)), 0.0, 1.0), 4.0);
	vec2 t = vec2(env.x, envOffsetY + env.y * envScaleY);
	vec4 textureOriginColor = texture2D(environment, t);
	vec4 textureColor = textureOriginColor;
	textureColor = clamp(vec4((textureColor.rgb - 0.75) * 2.0 + 0.5, textureColor.a), 0.0, 1.0);
	float l = (textureColor.r +  textureColor.g +  textureColor.b)/3.0;
	textureColor = vec4(l, l, l, textureColor.a);
	//gl_FragColor = mix(textureColor, vec4(0.4, 0.6, 0.7, 1.0), mixLevel);
	gl_FragColor = mix(textureOriginColor, textureColor * vec4(0.4, 0.6, 0.7, 1.0), mixLevel);
}