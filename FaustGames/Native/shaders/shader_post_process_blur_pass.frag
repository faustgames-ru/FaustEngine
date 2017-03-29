#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
 
varying vec2 _texCoord0;
varying vec2 _texCoord1;
varying vec2 _texCoord;
varying vec2 _texCoord2;
varying vec2 _texCoord3;
 
void main()
{
    gl_FragColor = 
		texture2D(texture, _texCoord0)*0.0625 + 
		texture2D(texture, _texCoord1)*0.25 + 
		texture2D(texture, _texCoord)*0.375+ 
		texture2D(texture, _texCoord2)*0.25 + 
		texture2D(texture, _texCoord3)*0.0625;
}