#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform vec2 pixelOffset;

attribute vec4 position;
attribute vec2 textureCoords; 

varying vec2 _texCoord0;
varying vec2 _texCoord1;
varying vec2 _texCoord;
varying vec2 _texCoord2;
varying vec2 _texCoord3;

void main()
{
    gl_Position = position;
    _texCoord0 = textureCoords-pixelOffset*2.0;
    _texCoord1 = textureCoords-pixelOffset*1.0;
    _texCoord = textureCoords;
    _texCoord2 = textureCoords+pixelOffset*1.0;
    _texCoord3 = textureCoords+pixelOffset*2.0;
}