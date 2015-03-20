attribute vec2 aPosition;
attribute float aVignette;
attribute vec2 aTextureCoord;

varying vec2 vTextureCoord;
varying float vVignette;

void main() {
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
    vTextureCoord = aTextureCoord.xy;
    vVignette = aVignette;
}
