#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
varying vec4 _color;
varying vec2 _textureCoords;



void main()
{
	//vec4 actualColor = texture2D(texture, _textureCoords);
	//vec4 replaceColor = vec4(
//		0.5, 
//		0.5, 
//		0.5, actualColor.a);
//	vec4 newColor = vec4(
//		216.0 / 255.0, 
//		144.0 / 255.0, 
//		17.0 / 255.0, actualColor.a);

//		newColor.rgb*=1.3;
		
//	float l = abs(actualColor.r - replaceColor.r)*2.0;// length(actualColor.rgb - replaceColor.rgb)*3.0;
//	gl_FragColor = mix(newColor, actualColor, min(l, 1.0));
	
	gl_FragColor = texture2D(texture, _textureCoords)*_color;
}