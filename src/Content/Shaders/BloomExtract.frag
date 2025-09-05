uniform float bloomThreshold;
uniform sampler2D texture;

void main()
{
    // Pixel color
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    // Saturate it to keep only values brighter than the specified threshold
    gl_FragColor = clamp((color - bloomThreshold) / (1.0 - bloomThreshold), 0.0, 1.0);
}