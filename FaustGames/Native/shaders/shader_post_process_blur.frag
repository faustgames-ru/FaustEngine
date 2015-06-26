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
varying vec2 _blurTexCoords04;
varying vec2 _blurTexCoords05;
varying vec2 _blurTexCoords06;
varying vec2 _blurTexCoords07;
varying vec2 _blurTexCoords08;
varying vec2 _blurTexCoords09;
varying vec2 _blurTexCoords10;
varying vec2 _blurTexCoords11;
varying vec2 _blurTexCoords12;
varying vec2 _blurTexCoords13;
 
void main()
{
    gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(texture, _blurTexCoords00)*0.0044299121055113265;
    gl_FragColor += texture2D(texture, _blurTexCoords01)*0.00895781211794;
    gl_FragColor += texture2D(texture, _blurTexCoords02)*0.0215963866053;
    gl_FragColor += texture2D(texture, _blurTexCoords03)*0.0443683338718;
    gl_FragColor += texture2D(texture, _blurTexCoords04)*0.0776744219933;
    gl_FragColor += texture2D(texture, _blurTexCoords05)*0.115876621105;
    gl_FragColor += texture2D(texture, _blurTexCoords06)*0.147308056121;
    gl_FragColor += texture2D(texture, _texCoord         )*0.159576912161;
    gl_FragColor += texture2D(texture, _blurTexCoords07)*0.147308056121;
    gl_FragColor += texture2D(texture, _blurTexCoords08)*0.115876621105;
    gl_FragColor += texture2D(texture, _blurTexCoords09)*0.0776744219933;
    gl_FragColor += texture2D(texture, _blurTexCoords10)*0.0443683338718;
    gl_FragColor += texture2D(texture, _blurTexCoords11)*0.0215963866053;
    gl_FragColor += texture2D(texture, _blurTexCoords12)*0.00895781211794;
    gl_FragColor += texture2D(texture, _blurTexCoords13)*0.0044299121055113265;
}