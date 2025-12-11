#version 130

uniform float saturationSigma;
uniform sampler2D texture;

out vec4 outColor;

void main()
{
    // Pixel color
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Saturate it to keep only values brighter than the specified threshold
    outColor = clamp((color - saturationSigma) / (1.0 - saturationSigma), 0.0, 1.0);
}