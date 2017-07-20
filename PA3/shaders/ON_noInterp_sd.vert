# version 120

attribute vec3 position;
attribute vec3 vNormal;
attribute vec3 texCoord;

uniform vec3 diffuseProduct;
uniform float albedo;
uniform float roughness;

uniform vec4 lightPosition;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

varying vec4 fColor, tex;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    tex = vec4 (texCoord, 1.0);

    vec3 pos = (chain * vec4(position, 1.0)).xyz;

    vec3 E = normalize( pos );
    vec3 N = normalize(chain * vec4(vNormal, 1.0)).xyz;


    vec3 L = normalize (lightPosition.xyz);

    float sig2 = pow(roughness, 2);
    float A = 1.0 - 0.5 * (sig2/(sig2+0.57));
    float B = 0.45 * (sig2/(sig2+0.09));

    float alpha = max(acos(dot(N, L)), acos(dot(N,E)));
    float beta = min(acos(dot(N, L)), acos(dot(N,E)));
    float gamma = dot(normalize(E - N * dot(E, N)), normalize(L - N * dot(L, N)));

    fColor = vec4(diffuseProduct, 1.0) * (A + B*max(0.0, gamma)* sin(alpha)*tan(beta)) * albedo * dot(N, L) / 3.1415;
    fColor.a = 1.0;

    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
}
