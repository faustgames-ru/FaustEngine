#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform samplerCube environment;

varying vec3 _normal;

void main()
{
	vec4 textureColor = textureCube(environment, _normal);
	gl_FragColor = textureColor;
}