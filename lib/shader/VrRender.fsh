varying mediump vec2 vTextureCoord;
varying mediump float vVignette;
uniform sampler2D uTextureSampler;

void main() {
    gl_FragColor = vVignette * texture2D(uTextureSampler, vTextureCoord);
}
