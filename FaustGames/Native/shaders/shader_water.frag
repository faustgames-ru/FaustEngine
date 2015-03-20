#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D environment;
uniform sampler2D depthmap;
uniform sampler2D normalmap;
uniform float normalScaleZ;
uniform float envOffsetY;
uniform float envScaleY;
uniform float reflectionBrightness;
uniform float reflectionContrast;
uniform float reflectionSaturation;
uniform vec4 reflectionTint0;
uniform vec4 reflectionTint1;

varying vec2 _textureCoords0;
varying vec2 _textureCoords1;
varying vec3 _eye;
varying vec2 _screen;
varying float _z;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 correctSaturation(vec3 c, float s)
{
  vec3 hsv = rgb2hsv(c);
  hsv.y *= s;
	return hsv2rgb(hsv);
}

float LinearizeDepth(float z)
{
  float n = 100.0;
  float f = 50000.0;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main()
{
	vec3 normal0 = texture2D(normalmap, _textureCoords0).xyz - 0.5;
	vec3 normal1 = texture2D(normalmap, _textureCoords1).xyz - 0.5;
	vec3 normal = normalize(normal0 + normal1);
	float depth = LinearizeDepth(texture2D(depthmap, _screen*0.5+0.5).r);
	float actualDepth = LinearizeDepth(_z);
	float alpha = clamp(abs(actualDepth - depth)*(50000.0-100)/500.0, 0.0, 1.0);
	
		
	normal.z *= normalScaleZ;
	normal = normalize(normal);
	vec3 env = normalize(reflect (_eye, normal));
	float mixLevel = 1.0 - pow(clamp((dot(_eye, normal)), 0.0, 1.0), 4.0);
	vec2 t = vec2(env.x, envOffsetY + env.y * envScaleY);
	vec4 textureOriginColor = texture2D(environment, t);
	vec4 textureColor = textureOriginColor;
	textureColor.rgb = correctSaturation(textureColor.rgb, reflectionSaturation);
	textureColor.rgb = (textureColor.rgb - 0.5) * reflectionContrast + 0.5;
	textureColor.rgb += reflectionBrightness;
	textureColor.rgb = clamp(textureColor.rgb, 0.0, 1.0);
	textureOriginColor.a = alpha;
	textureColor.a = alpha;
	//gl_FragColor = mix(textureOriginColor, textureColor * vec4(0.4, 0.6, 0.7, 1.0), mixLevel);
	gl_FragColor = mix(textureOriginColor * reflectionTint0, textureColor * reflectionTint1, mixLevel);
	
}