#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform vec2 pixelOffset;

attribute vec4 position;
attribute vec2 textureCoords; 

varying vec2 _texCoord0;
varying vec2 _texCoord;
varying vec2 _texCoord1;

void main()
{
    gl_Position = position;
    _texCoord0 = textureCoords-pixelOffset;
    _texCoord = textureCoords;
    _texCoord1 = textureCoords+pixelOffset;
}