uniform sampler2D MainTexture;
uniform float width;
uniform float height;
uniform float glow;
uniform float sigma;

float blurWeight(float x)
{
    return (exp(-(x*x) / (2.0 * sigma * sigma)));
}

void main()
{
    vec4 color = vec4(0.0);
    vec2 texCoord = gl_TexCoord[0].xy;

    int bz = 10;

    float bw = 0.0;
    for (int i = -bz; i <= bz; i++)
    {
        texCoord.y = gl_TexCoord[0].y + (float(i) / height);

        for (int j = -bz; j <= bz; j++)
        {
            texCoord.x = gl_TexCoord[0].x + (float(j) / width);
            float bwc = blurWeight(sqrt(float(i)*float(i) + float(j)*float(j)));
            bw += bwc;
            color += texture2D(MainTexture, texCoord) * bwc;
        }
    }

    color *= glow;
    color /= bw;

    color -= 0.00;

    gl_FragColor = color;
}