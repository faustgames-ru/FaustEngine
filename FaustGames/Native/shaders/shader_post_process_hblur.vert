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
    _blurTexCoords[ 0] = _texCoord - pixelSize * vec2(7.0, 0.0);
    _blurTexCoords[ 1] = _texCoord - pixelSize * vec2(6.0, 0.0);
    _blurTexCoords[ 2] = _texCoord - pixelSize * vec2(5.0, 0.0);
    _blurTexCoords[ 3] = _texCoord - pixelSize * vec2(4.0, 0.0);
    _blurTexCoords[ 4] = _texCoord - pixelSize * vec2(3.0, 0.0);
    _blurTexCoords[ 5] = _texCoord - pixelSize * vec2(2.0, 0.0);
    _blurTexCoords[ 6] = _texCoord - pixelSize * vec2(1.0, 0.0);
    _blurTexCoords[ 7] = _texCoord + pixelSize * vec2(1.0, 0.0);
    _blurTexCoords[ 8] = _texCoord + pixelSize * vec2(2.0, 0.0);
    _blurTexCoords[ 9] = _texCoord + pixelSize * vec2(3.0, 0.0);
    _blurTexCoords[10] = _texCoord + pixelSize * vec2(4.0, 0.0);
    _blurTexCoords[11] = _texCoord + pixelSize * vec2(5.0, 0.0);
    _blurTexCoords[12] = _texCoord + pixelSize * vec2(6.0, 0.0);
    _blurTexCoords[13] = _texCoord + pixelSize * vec2(7.0, 0.0);
}