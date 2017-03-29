#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
 
varying vec2 _texCoord0;
varying vec2 _texCoord;
varying vec2 _texCoord1;
 
void main()
{
    gl_FragColor = (texture2D(texture, _texCoord0) + 
		texture2D(texture, _texCoord)*2.0+ 
		texture2D(texture, _texCoord1)) / 4.001;
}