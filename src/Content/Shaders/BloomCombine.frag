uniform float bloomIntensity;
uniform float bloomSaturation;
uniform float baseIntensity;
uniform float baseSaturation;
uniform sampler2D bloomTexture;
uniform sampler2D baseTexture;

vec4 adjustSaturation(vec4 c, float saturation) {

    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    float gray = dot(vec3(c.x, c.y, c.z), vec3(0.3, 0.59, 0.11));

    return mix(vec4(gray), c, saturation);
}

void main() {

    // Original colors
    vec4 bloom = texture2D(bloomTexture, gl_TexCoord[0].xy);
    vec4 base = texture2D(baseTexture, gl_TexCoord[0].xy);

    // Adjust color saturation and intensity
    bloom = adjustSaturation(bloom, bloomSaturation) * bloomIntensity;
    base = adjustSaturation(base, baseSaturation) * baseIntensity;

    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out
    // base *= 1.0 - clamp(bloom, 0.0, 1.0);

    // Combine the two colors
    gl_FragColor = bloom + base;
}