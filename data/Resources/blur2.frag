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
        texCoord.y = gl_TexCoord[0].y + (float(i) / width);
        color += texture2D(MainTexture, texCoord) * blurWeight(float(i));
    }

    int i = 0;
    for(;i<5 && texCoord.x*texCoord.x + texCoord.y*texCoord.y < 2.0*2.0;i++) {
    	texCoord = vec2(texCoord.x*texCoord.x - texCoord.y*texCoord.y + gl_TexCoord[0].x, 2.0*texCoord.x*texCoord.y + gl_TexCoord[0].y);
    }

    color = color + vec4(log(float(i))/7.0,0,0,1);
    gl_FragColor = color;
}