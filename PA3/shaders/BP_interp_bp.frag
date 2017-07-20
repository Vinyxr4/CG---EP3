# version 120

uniform vec3 ambientProduct;
uniform vec3 diffuseProduct;
uniform vec3 specularProduct;

uniform float shininess, bumpness;

uniform sampler2D texture;

varying vec3 N, E, L, tangent;
varying vec4 tex;

void main()
{
    vec4 col;
    vec4 t = texture2D(texture, tex.st);

    vec3 t1 = tangent;
    vec3 t2 = cross(t1,N);

    vec3 texCoef = t.rgb - vec3(0.5,0.5,0.5);
    vec3 N2 = normalize(bumpness*t1*texCoef.r + bumpness*cross(t1,N)*texCoef.g + N);

    vec3 E2 = normalize (E);
    vec3 H = normalize (L + E2);

    vec4 ambient = vec4(ambientProduct, 1.0);
    vec4 diffuse = vec4(diffuseProduct, 1.0) * max( dot(L, N2), 0.0 );
    vec4 specular = vec4(specularProduct, 1.0) * pow( max(dot(N2, H), 0.0), shininess );

    col = ambient + diffuse +specular;
    col.a = 1.0;

    gl_FragColor = col;
}

