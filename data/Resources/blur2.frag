uniform sampler2D MainTexture;
uniform float sigma;
uniform float glowMultiplier;
uniform float width;
uniform float blurSize;
uniform vec2 p1;
uniform vec2 p2;
uniform float lambda1;
uniform float lambda2;
uniform float str1;
uniform float str2;
uniform float time;

//const int KERNEL_SIZE = 20;
float glow = glowMultiplier / (sigma * sqrt(2.0 * 3.14159));

int bz = int(blurSize);

float blurWeight(float x)
{
    return (glow * exp(-(x*x) / (2.0 * sigma * sigma)));
}

void main()
{
    /*vec4 color = vec4(0.0);
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
    gl_FragColor = color;*/


    float d1 = distance(gl_TexCoord[0].xy,p1);
    float d2 = distance(gl_TexCoord[0].xy,p2);

    float wave1 = str1*sin((d1/lambda1)*3.141592 - time);
    float wave2 = str2*sin((d2/lambda2)*3.141592 - time);

    float s = wave1+wave2;

    vec4 color = vec4(s,-s,0.0,1.0);
    gl_FragColor = color;
}