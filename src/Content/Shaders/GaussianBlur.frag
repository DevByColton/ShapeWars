#version 130

uniform int sampleCount;
uniform float sampleWeights[15];
uniform vec2 sampleOffsets[15];
uniform sampler2D texture;

out vec4 outColor;

void main()
{
    vec4 c = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < sampleCount; i++) {
        c += (texture2D(texture, gl_TexCoord[0].xy + sampleOffsets[i])) * sampleWeights[i];
    }

    outColor = c;
}