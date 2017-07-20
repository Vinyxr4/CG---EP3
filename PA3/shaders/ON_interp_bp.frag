# version 120

uniform vec3 diffuseProduct;
uniform float albedo;
uniform float roughness;
uniform float bumpness;

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

    float sig2 = pow(roughness, 2);
    float A = 1.0 - 0.5 * (sig2/(sig2+0.57));
    float B = 0.45 * (sig2/(sig2+0.09));

    float alpha = max(acos(dot(N2, L)), acos(dot(N2,E)));
    float beta = min(acos(dot(N2, L)), acos(dot(N2,E)));
    float gamma = dot(normalize(E - N2 * dot(E, N2)), normalize(L - N2 * dot(L, N2)));

    col = vec4(diffuseProduct, 1.0) * (A + B*max(0.0, gamma)* sin(alpha)*tan(beta)) * albedo * dot(N2, L) / 3.1415;
    col.a = 1.0;

    gl_FragColor = col;
}

