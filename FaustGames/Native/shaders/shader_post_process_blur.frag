#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
 
varying vec2 _texCoord;

varying vec2 _blurTexCoords00;
varying vec2 _blurTexCoords01;
varying vec2 _blurTexCoords02;
varying vec2 _blurTexCoords03;
 
void main()
{
    gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(texture, _blurTexCoords00)*0.0625;
    gl_FragColor += texture2D(texture, _blurTexCoords01)*0.25;
    gl_FragColor += texture2D(texture, _texCoord)*0.375;
    gl_FragColor += texture2D(texture, _blurTexCoords02)*0.25;
    gl_FragColor += texture2D(texture, _blurTexCoords03)*0.0625;
}