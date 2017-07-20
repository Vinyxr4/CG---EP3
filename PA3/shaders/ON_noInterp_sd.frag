# version 120

uniform sampler2D texture;

varying vec4 fColor, tex;

void main()
{
    vec4 t = texture2D(texture, tex.st);
    gl_FragColor = fColor * t;
}

