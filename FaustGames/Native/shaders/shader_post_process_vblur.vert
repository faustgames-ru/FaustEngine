#ifdef GL_ES
precision highp float;
precision mediump int;
#endif
uniform vec2 pixelSize;
attribute vec4 position;
attribute vec2 textureCoords; 
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
    gl_Position=position;
    _texCoord=textureCoords;
    _blurTexCoords00=_texCoord-vec2(0.0,7.0)*pixelSize;
    _blurTexCoords01=_texCoord-vec2(0.0,6.0)*pixelSize;
    _blurTexCoords02=_texCoord-vec2(0.0,5.0)*pixelSize;
    _blurTexCoords03=_texCoord-vec2(0.0,4.0)*pixelSize;
    _blurTexCoords04=_texCoord-vec2(0.0,3.0)*pixelSize;
    _blurTexCoords05=_texCoord-vec2(0.0,2.0)*pixelSize;
    _blurTexCoords06=_texCoord-vec2(0.0,1.0)*pixelSize;
    _blurTexCoords07=_texCoord+vec2(0.0,1.0)*pixelSize;
    _blurTexCoords08=_texCoord+vec2(0.0,2.0)*pixelSize;
    _blurTexCoords09=_texCoord+vec2(0.0,3.0)*pixelSize;
    _blurTexCoords10=_texCoord+vec2(0.0,4.0)*pixelSize;
    _blurTexCoords11=_texCoord+vec2(0.0,5.0)*pixelSize;
    _blurTexCoords12=_texCoord+vec2(0.0,6.0)*pixelSize;
    _blurTexCoords13=_texCoord+vec2(0.0,7.0)*pixelSize;
}