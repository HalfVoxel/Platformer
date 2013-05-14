/*uniform sampler2D MainTexture;
uniform float sigma;
uniform float glowMultiplier;
uniform float width;*/

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}