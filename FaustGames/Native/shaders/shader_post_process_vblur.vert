#ifdef GL_ES
precision highp float;
precision mediump int;
#endif

uniform vec2 pixelSize;

attribute vec4 position;
attribute vec2 textureCoords;
 
varying vec2 _texCoord;
varying vec2 _blurTexCoords[14];
 
void main()
{
    gl_Position = position;
    _texCoord = textureCoords;
    _blurTexCoords[ 0] = _texCoord - vec2(0.0,7.0)*pixelSize;
    _blurTexCoords[ 1] = _texCoord - vec2(0.0,6.0)*pixelSize;
    _blurTexCoords[ 2] = _texCoord - vec2(0.0,5.0)*pixelSize;
    _blurTexCoords[ 3] = _texCoord - vec2(0.0,4.0)*pixelSize;
    _blurTexCoords[ 4] = _texCoord - vec2(0.0,3.0)*pixelSize;
    _blurTexCoords[ 5] = _texCoord - vec2(0.0,2.0)*pixelSize;
    _blurTexCoords[ 6] = _texCoord - vec2(0.0,1.0)*pixelSize;
    _blurTexCoords[ 7] = _texCoord + vec2(0.0,1.0)*pixelSize;
    _blurTexCoords[ 8] = _texCoord + vec2(0.0,2.0)*pixelSize;
    _blurTexCoords[ 9] = _texCoord + vec2(0.0,3.0)*pixelSize;
    _blurTexCoords[10] = _texCoord + vec2(0.0,4.0)*pixelSize;
    _blurTexCoords[11] = _texCoord + vec2(0.0,5.0)*pixelSize;
    _blurTexCoords[12] = _texCoord + vec2(0.0,6.0)*pixelSize;
    _blurTexCoords[13] = _texCoord + vec2(0.0,7.0)*pixelSize;
}