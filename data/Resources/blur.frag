uniform sampler2D MainTexture;
uniform float sigma;
uniform float glowMultiplier;
uniform float width;
uniform float blurSize;
//const int KERNEL_SIZE = 20;
float glow = glowMultiplier / (sigma * sqrt(2.0 * 3.14159));

int bz = int(blurSize);

float blurWeight(float x)
{
    return (glow * exp(-(x*x) / (2.0 * sigma * sigma)));
}

void main()
{
    vec4 color = vec4(0.0);
    vec2 texCoord = gl_TexCoord[0].xy;

    for (int i = -bz; i <= bz; i++)
    {
        texCoord.x = gl_TexCoord[0].x + (float(i) / width);
        color += texture2D(MainTexture, texCoord) * blurWeight(float(i));
    }


    
    gl_FragColor = color;
}