# version 120

uniform sampler2D texture;

varying vec4 fColor, t;

void main()
{
    gl_FragColor = fColor * t;
}

