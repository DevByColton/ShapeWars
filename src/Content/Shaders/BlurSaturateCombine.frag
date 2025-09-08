uniform sampler2D baseTexture;
uniform sampler2D saturationTexture;

void main() {

    // Original colors
    vec4 saturation = texture2D(saturationTexture, gl_TexCoord[0].xy);
    vec4 base = texture2D(baseTexture, gl_TexCoord[0].xy);

    // Combine the two colors
    gl_FragColor = saturation + base;
}