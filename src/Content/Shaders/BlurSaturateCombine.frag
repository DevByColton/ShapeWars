#version 130

uniform sampler2D baseTexture;
uniform sampler2D saturationTexture;

out vec4 outColor;

void main() {

    // Original colors
    vec4 saturation = texture2D(saturationTexture, gl_TexCoord[0].xy);
    vec4 base = texture2D(baseTexture, gl_TexCoord[0].xy);

    // Combine the two colors
    outColor = saturation + base;
}