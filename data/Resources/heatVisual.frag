uniform sampler2D MainTexture;

void main()
{

    vec4 color = texture2D (MainTexture, gl_TexCoord[0].xy);
    color.g = 3.0*color.r - 1.0;
    color.b = color.g - 1.0;
    gl_FragColor = color;
}