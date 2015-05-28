#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

uniform sampler2D texture;
 
varying vec2 _texCoord;
varying vec2 _blurTexCoords[14];
 
void main()
{
    gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(texture, _blurTexCoords[ 0])*0.0044299121055113265;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 1])*0.00895781211794;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 2])*0.0215963866053;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 3])*0.0443683338718;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 4])*0.0776744219933;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 5])*0.115876621105;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 6])*0.147308056121;
    gl_FragColor += texture2D(texture, _texCoord         )*0.159576912161;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 7])*0.147308056121;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 8])*0.115876621105;
    gl_FragColor += texture2D(texture, _blurTexCoords[ 9])*0.0776744219933;
    gl_FragColor += texture2D(texture, _blurTexCoords[10])*0.0443683338718;
    gl_FragColor += texture2D(texture, _blurTexCoords[11])*0.0215963866053;
    gl_FragColor += texture2D(texture, _blurTexCoords[12])*0.00895781211794;
    gl_FragColor += texture2D(texture, _blurTexCoords[13])*0.0044299121055113265;
}