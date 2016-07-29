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

void main()
{
    gl_Position = position;
    _texCoord = textureCoords;
    _blurTexCoords00 = _texCoord - vec2(0.0, 2.0) * pixelSize;
    _blurTexCoords01 = _texCoord - vec2(0.0, 1.0) * pixelSize;
    _blurTexCoords02 = _texCoord + vec2(0.0, 1.0) * pixelSize;
    _blurTexCoords03 = _texCoord + vec2(0.0, 2.0) * pixelSize;    
}